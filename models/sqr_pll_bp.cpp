//
//  File = sqr_pll_bp.cpp
//

//#include <stdlib.h>
//#include <fstream.h>
#include "parmfile.h"
#include "misdefs.h"
#include "model_error.h"
#include "sqr_pll_bp.h"
#include "butt_filt_iir.h"
#include "model_graph.h"
//#include "sinc.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//======================================================

BandpassSquaringPLL::BandpassSquaringPLL( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* fsig_input,
                        Signal<float>* fsig_square_of_input,
                        Signal<float>* fsig_phase_error,
                        Signal<float>* fsig_filtered_error,
                        Signal<float>* fsig_vco_output,
                        Signal<float>* vco_freq_sig,
                        Signal<float>* ref_phase_sig,
                        Signal<float>* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
   MODEL_NAME(BandpassSquaringPLL);

   //---------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;

   GET_DOUBLE_PARM(K_Sub_D);
   GET_DOUBLE_PARM(K_Sub_0);
   GET_DOUBLE_PARM(Center_Freq_Hz);
   GET_BOOL_PARM(m_UsingDco);

   //--------------------------------------
   //  Connect input and output signals

   fsig_Input = fsig_input;
   fsig_Phase_Error = fsig_phase_error;
   fsig_Filtered_Error = fsig_filtered_error;
   fsig_Osc_Output = fsig_vco_output;
   fsig_Osc_Freq = vco_freq_sig;
   fsig_Osc_Phase = ref_phase_sig;
   fsig_Output = out_sig;
   fsig_Square_Of_Input = fsig_square_of_input;

   MAKE_OUTPUT( fsig_Output );
   MAKE_OUTPUT( fsig_Phase_Error );
   MAKE_OUTPUT( fsig_Filtered_Error );
   MAKE_OUTPUT( fsig_Osc_Output );
   MAKE_OUTPUT( fsig_Osc_Freq );
   MAKE_OUTPUT( fsig_Osc_Phase );
   MAKE_OUTPUT( fsig_Square_Of_Input );
   
   MAKE_INPUT( fsig_Input );

   //----------------------------------
   //  Compute derived parameters

   Omega_Sub_0 = TWO_PI * Center_Freq_Hz;
   //--------------------------------
   // create loop filter object
   char sub_name[50];
   strcpy(sub_name, GetInstanceName());
   strcat(sub_name, ":Filter\0");
   Filter_Core = new ButterworthFilterByIir<float>( sub_name, this);
}

//======================================
BandpassSquaringPLL::~BandpassSquaringPLL( void ){ };

//=======================================
void BandpassSquaringPLL::Initialize(void)
{
   //------------------
   int block_size = fsig_Output->GetBlockSize();
   Samp_Intvl = fsig_Input->GetSampIntvl();
   Filter_Core->Initialize(block_size, Samp_Intvl);
   Osc_Output_Prev_Val = 0.0;
   OscOutput = 0;
   Phi_Sub_2 = 0;

}
//=======================================================
int BandpassSquaringPLL::Execute()
{
   // pointers for signal data

   float *fsOutput_ptr;
   float *fsPhaseError_ptr;
   float *fs_filtered_error_ptr;
   float *fsOscOutput_ptr;
   float *fsOscFreq_ptr;
   float *fsOscPhase_ptr;
   float *fsInput_ptr;
   float *fsSquareOfInput_ptr;

   float input_val;
   float osc_output_val;
   float filt_val;
   float inst_freq;

   double samp_intvl;
   double phase_error;
   double err_sum=0;
   double err_avg;
   int is;
   int block_size;


   //--------------------------------------------------------------
   // set up pointers to data buffers for input and output signals

   fsOutput_ptr = GET_OUTPUT_PTR( fsig_Output );
   fsPhaseError_ptr = GET_OUTPUT_PTR( fsig_Phase_Error );
   fs_filtered_error_ptr = GET_OUTPUT_PTR( fsig_Filtered_Error );

   fsOscOutput_ptr = GET_OUTPUT_PTR( fsig_Osc_Output );
   fsOscFreq_ptr = GET_OUTPUT_PTR( fsig_Osc_Freq );
   fsOscPhase_ptr = GET_OUTPUT_PTR( fsig_Osc_Phase );
   fsSquareOfInput_ptr = GET_OUTPUT_PTR( fsig_Square_Of_Input );
   fsInput_ptr = GET_INPUT_PTR( fsig_Input );
   //---------------------------------------------------------------

   samp_intvl = Samp_Intvl;
   osc_output_val = Osc_Output_Prev_Val;

   block_size = fsig_Input->GetValidBlockSize();

   fsig_Output->SetValidBlockSize(block_size);
   fsig_Phase_Error->SetValidBlockSize(block_size);
   fsig_Filtered_Error->SetValidBlockSize(block_size);
   fsig_Osc_Output->SetValidBlockSize(block_size);
   fsig_Osc_Freq->SetValidBlockSize(block_size);
   fsig_Osc_Phase->SetValidBlockSize(block_size);
   fsig_Square_Of_Input->SetValidBlockSize(block_size);

   //-------------------------------------------------------

   for (is=0; is<block_size; is++)
   {
      *fsOscOutput_ptr++ = osc_output_val;
      input_val = *fsInput_ptr;
      *fsSquareOfInput_ptr++ = input_val * input_val;
      phase_error = K_Sub_D * input_val * input_val * OscOutput;
//      phase_error = K_Sub_D * input_val * OscOutput;
      *fsPhaseError_ptr++ = phase_error;

      //--------------------------------
      //  filter the error signal

      filt_val = Filter_Core->ProcessSample(phase_error);
      *fs_filtered_error_ptr++ = filt_val;

      //---------------------------------------
      //  just for educational purposes, 
      //  compute the average of the filtered error signal

      err_sum += filt_val;

      //----------------------------------------
      // use filtered error signal to drive VCO

      inst_freq = Omega_Sub_0 + K_Sub_0 * filt_val;
      Phi_Sub_2 += inst_freq * samp_intvl;
      if(Phi_Sub_2 > PI) Phi_Sub_2 -= TWO_PI;

      if(m_UsingDco) // make discrete valued output
      {
         if( (Phi_Sub_2 < -PI_OVER_TWO) || (Phi_Sub_2 > PI_OVER_TWO))
         {
            OscOutput = -1;
         }
         else
         {
            OscOutput = 1;
         }
      }
      else // simulate analog VCO output
      {
         OscOutput = cos(Phi_Sub_2);
      }

      *fsOscPhase_ptr++ = Phi_Sub_2;
      *fsOscFreq_ptr++ = inst_freq/TWO_PI;
      *fsOutput_ptr++ = sin(Phi_Sub_2);

      fsInput_ptr++;
   }
   err_avg = err_sum / block_size;
   BasicResults << "avg PLL error = " << err_avg << endl;
   Osc_Output_Prev_Val = osc_output_val;

  return(_MES_AOK);
}

