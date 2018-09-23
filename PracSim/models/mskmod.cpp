//
//  File = mskmod.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "mskmod.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
using std::complex;

//======================================================

MskModulator::MskModulator( 
               char* instance_name,
               PracSimModel* outer_model,
               Signal< float >* i_in_sig,
               Signal< float >* q_in_sig,
               Signal< complex<float> >* cmpx_out_sig,
               Signal< float >* mag_out_sig,
               Signal< float >* phase_out_sig )
          :PracSimModel(instance_name,
                        outer_model)
{
   MODEL_NAME(MskModulator);

   //  Read model config parms
   OPEN_PARM_BLOCK;
   GET_DOUBLE_PARM(Bit_Durat);
   GET_DOUBLE_PARM(Data_Skew);
   GET_DOUBLE_PARM(Subcar_Misalign);
   GET_DOUBLE_PARM(Phase_Unbal);
   GET_DOUBLE_PARM(Amp_Unbal);
   GET_BOOL_PARM(Shaping_Is_Bipolar);

   //  Connect input and output signals
   I_In_Sig = i_in_sig;
   Q_In_Sig = q_in_sig;
   Cmpx_Out_Sig = cmpx_out_sig;
   Mag_Out_Sig = mag_out_sig;
   Phase_Out_Sig = phase_out_sig;

   MAKE_OUTPUT( Cmpx_Out_Sig );
   MAKE_OUTPUT( Mag_Out_Sig );
   MAKE_OUTPUT( Phase_Out_Sig );
   MAKE_INPUT( I_In_Sig );
   MAKE_INPUT( Q_In_Sig );

   // Set up derived parms
   double phase_unbal_rad = PI * Phase_Unbal / 180.0;
   Pi_Over_Bit_Dur = PI/Bit_Durat;
   Phase_Shift = complex<float>( -sin(phase_unbal_rad),
                                 cos(phase_unbal_rad));
}
//======================================================
MskModulator::~MskModulator( void ){ };
//======================================================
void MskModulator::Initialize(void)
{
   Samps_Out_Cnt = 0;
   Block_Size = I_In_Sig->GetBlockSize();
   Samp_Intvl = I_In_Sig->GetSampIntvl();
}
//======================================================
int MskModulator::Execute(void)
{
   float *i_in_sig_ptr, *q_in_sig_ptr;
   float *phase_out_sig_ptr, *mag_out_sig_ptr;
   float subcar_misalign, amp_unbal, data_skew;
   float work, work1;
   std::complex<float> work2;
   std::complex<float> *cmpx_out_sig_ptr;
   int samps_out_cnt;
   double samp_intvl;
   double pi_over_bit_dur, argument;
   std::complex<float> phase_shift;
   long int_mult;
   int shaping_is_bipolar;
   int block_size;
   int is;

   cmpx_out_sig_ptr = GET_OUTPUT_PTR( Cmpx_Out_Sig );
   phase_out_sig_ptr = GET_OUTPUT_PTR( Phase_Out_Sig );
   mag_out_sig_ptr = GET_OUTPUT_PTR( Mag_Out_Sig );
   i_in_sig_ptr = GET_INPUT_PTR( I_In_Sig );
   q_in_sig_ptr = GET_INPUT_PTR( Q_In_Sig );

   samps_out_cnt = Samps_Out_Cnt;
   samp_intvl = Samp_Intvl;
   subcar_misalign = Subcar_Misalign;
   amp_unbal = Amp_Unbal;
   data_skew = Data_Skew;
   pi_over_bit_dur = Pi_Over_Bit_Dur;
   phase_shift = Phase_Shift;
   shaping_is_bipolar = Shaping_Is_Bipolar;

   block_size = I_In_Sig->GetValidBlockSize();
   Cmpx_Out_Sig->SetValidBlockSize(block_size);
   Mag_Out_Sig->SetValidBlockSize(block_size);
   Phase_Out_Sig->SetValidBlockSize(block_size);

   for (is=0; is<block_size; is++){
      argument = pi_over_bit_dur * samps_out_cnt *
                  samp_intvl;
      int_mult = long(argument/TWO_PI);
      argument -= int_mult*TWO_PI;

      if(shaping_is_bipolar){
         work1 = float((*i_in_sig_ptr) 
            * sin(argument-pi_over_bit_dur*
            subcar_misalign));
         work = float(amp_unbal * (*q_in_sig_ptr) 
            * cos(argument-pi_over_bit_dur
            *(subcar_misalign+data_skew)));
      }
      else{
         work1 = float((*i_in_sig_ptr) 
            * fabs(sin(argument-pi_over_bit_dur*
            subcar_misalign)));
         work = float(amp_unbal * (*q_in_sig_ptr) 
            * fabs(cos(argument-pi_over_bit_dur
            *(subcar_misalign+data_skew))));
      }
      work2 = work * phase_shift;
      *cmpx_out_sig_ptr = work1 + work2;
      *phase_out_sig_ptr = float(180.0*
               std::arg<float>(*cmpx_out_sig_ptr)/PI);
      *mag_out_sig_ptr = 
               std::abs<float>(*cmpx_out_sig_ptr);
      samps_out_cnt++;
      i_in_sig_ptr++;
      q_in_sig_ptr++;
      cmpx_out_sig_ptr++;
      phase_out_sig_ptr++;
      mag_out_sig_ptr++;
   }
   Samps_Out_Cnt = samps_out_cnt;
   return(_MES_AOK);
}

