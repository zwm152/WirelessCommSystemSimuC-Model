//
//  File = costas_loop.cpp
//

//#include <stdlib.h>
//#include <fstream.h>
#include "parmfile.h"
#include "misdefs.h"
#include "model_error.h"
#include "costas_loop.h"
#include "butt_filt_iir.h"
#include "model_graph.h"
//#include "sinc.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//======================================================

CostasLoop::CostasLoop( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* fsig_input,
                        Signal<float>* fsig_phase_error,
                        Signal<float>* fsig_filtered_error,
                        Signal<float>* fsig_vco_output,
                        Signal<float>* vco_freq_sig,
                        Signal<float>* ref_phase_sig,
                        Signal<float>* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
   MODEL_NAME(CostasLoop);

   //---------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;

   GET_DOUBLE_PARM(K_Sub_D);
   GET_DOUBLE_PARM(K_Sub_0);
   GET_DOUBLE_PARM(Center_Freq_Hz);
   GET_BOOL_PARM(m_UsingDco);
   //GET_DOUBLE_PARM(Scaler_Divisor);
   Scaler_Divisor = 1.0;

   //--------------------------------------
   //  Connect input and output signals

   fsig_Input = fsig_input;
   fsig_Phase_Error = fsig_phase_error;
   fsig_Filtered_Error = fsig_filtered_error;
   fsig_Osc_Output = fsig_vco_output;
   fsig_Osc_Freq = vco_freq_sig;
   fsig_Osc_Phase = ref_phase_sig;
   fsig_Output = out_sig;

   MAKE_OUTPUT( fsig_Output );
   MAKE_OUTPUT( fsig_Phase_Error );
   MAKE_OUTPUT( fsig_Filtered_Error );
   MAKE_OUTPUT( fsig_Osc_Output );
   MAKE_OUTPUT( fsig_Osc_Freq );
   MAKE_OUTPUT( fsig_Osc_Phase );
   MAKE_INPUT( fsig_Input );

   //----------------------------------
   //  Compute derived parameters

   Omega_Sub_0 = TWO_PI * Center_Freq_Hz;
   //--------------------------------
   // create loop filter object
   char sub_name[50];
   strcpy(sub_name, GetInstanceName());
   strcat(sub_name, ":I_Filter\0");
   I_Filter_Core = new ButterworthFilterByIir<float>( sub_name, this);

   strcpy(sub_name, GetInstanceName());
   strcat(sub_name, ":Q_Filter\0");
   Q_Filter_Core = new ButterworthFilterByIir<float>( sub_name, this);

   strcpy(sub_name, GetInstanceName());
   strcat(sub_name, ":Prod_Filter\0");
   Prod_Filter_Core = new ButterworthFilterByIir<float>( sub_name, this);
}

//======================================
CostasLoop::~CostasLoop( void ){ };

//=======================================
void CostasLoop::Initialize(void)
{
   //------------------
   int block_size = fsig_Output->GetBlockSize();
   Samp_Intvl = fsig_Input->GetSampIntvl();
   I_Filter_Core->Initialize(block_size, Samp_Intvl);
   Q_Filter_Core->Initialize(block_size, Samp_Intvl);
   Prod_Filter_Core->Initialize(block_size, Samp_Intvl);
   Osc_Output_Prev_Val = 0.0;
   I_OscOutput = 0;
   Q_OscOutput = 0;
   Phi_Sub_2 = 0;
   Phi_Divided = 0;

}
//=======================================================
int CostasLoop::Execute()
{
   // pointers for signal data

   float *fsOutput_ptr;
   float *fsPhaseError_ptr;
   float *fs_filtered_error_ptr;
   float *fsOscOutput_ptr;
   float *fsOscFreq_ptr;
   float *fsOscPhase_ptr;
   float *fsInput_ptr;

   float input_val;
   float osc_output_val;
   float i_filt_val;
   float q_filt_val;
   float inst_freq;

   double samp_intvl;
   double error_val;
   double filt_val;
   double i_arm_product;
   double q_arm_product;
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

   //-------------------------------------------------------

   for (is=0; is<block_size; is++)
   {
      *fsOscOutput_ptr++ = osc_output_val;
      input_val = *fsInput_ptr;
      i_arm_product = K_Sub_D * input_val * I_OscOutput;
      q_arm_product = K_Sub_D * input_val * Q_OscOutput;
      *fsPhaseError_ptr++ = i_arm_product;

      //--------------------------------
      //  filter the arm signals

      i_filt_val = I_Filter_Core->ProcessSample(i_arm_product);
      *fs_filtered_error_ptr++ = i_filt_val;
      q_filt_val = Q_Filter_Core->ProcessSample(q_arm_product);

      //----------------------------------------
      //  multiply arm signals

      error_val = i_filt_val * q_filt_val;

      //----------------------------------------
      //  filter the result

      filt_val = Prod_Filter_Core->ProcessSample(error_val);

      //----------------------------------------
      // use filtered error signal to drive VCO

      inst_freq = Omega_Sub_0 + K_Sub_0 * filt_val;
      Phi_Sub_2 += inst_freq * samp_intvl;
      if(Phi_Sub_2 > PI) Phi_Sub_2 -= TWO_PI;

      I_OscOutput = -sin(Phi_Sub_2);
      Q_OscOutput = -cos(Phi_Sub_2);

      *fsOscPhase_ptr++ = Phi_Sub_2;
      *fsOscFreq_ptr++ = inst_freq/TWO_PI;

      Phi_Divided += inst_freq * samp_intvl/Scaler_Divisor;
      if(Phi_Divided > PI) Phi_Divided -= TWO_PI;
      *fsOutput_ptr++ = sin(Phi_Divided);

      fsInput_ptr++;
   }
   err_avg = err_sum / block_size;
   BasicResults << "avg PLL error = " << err_avg << endl;
   Osc_Output_Prev_Val = osc_output_val;

  return(_MES_AOK);
}

