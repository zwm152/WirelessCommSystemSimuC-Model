//
//  File = nonlinear_amp.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "nonlinear_amp.h"
#include "model_graph.h"
extern ParmFile *ParmInput;

//======================================================

NonlinearAmplifier::NonlinearAmplifier( 
                     char* instance_name,
                     PracSimModel* outer_model,
                     Signal< complex<float> >* in_sig,
                     Signal< complex<float> >* out_sig )
             :PracSimModel(instance_name,
                           outer_model)
{
  MODEL_NAME(NonlinearAmplifier);

  //  Read model config parms

   OPEN_PARM_BLOCK;
   GET_DOUBLE_PARM(Output_Power_Scale_Factor);
   GET_DOUBLE_PARM(Phase_Scale_Factor);
   GET_DOUBLE_PARM(Anticipated_Input_Power);
   GET_DOUBLE_PARM(Operating_Point);
   GET_DOUBLE_PARM(Agc_Time_Constant);

   Input_Power_Scale_Factor = 
         float(Operating_Point/Anticipated_Input_Power);

   Am_Am_Fname = new char[64];
   strcpy(Am_Am_Fname, "\0");
   GET_STRING_PARM(Am_Am_Fname);

   Am_Pm_Fname = new char[64];
   strcpy(Am_Pm_Fname, "\0");
   GET_STRING_PARM(Am_Pm_Fname);

  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;
  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  Am_Am_Curve = new SampledCurve(Am_Am_Fname);
  Am_Pm_Curve = new SampledCurve(Am_Pm_Fname);
  }

//======================================================
void NonlinearAmplifier::Initialize(void)
{
  double samp_intvl = Out_Sig->GetSampIntvl();
  Out_Avg_Block_Size = Out_Sig->GetBlockSize();
}
//======================================================
NonlinearAmplifier::~NonlinearAmplifier( void ){ };

//======================================================
int NonlinearAmplifier::Execute()
{
   complex<float>  *out_sig_ptr, out_sig;
   complex<float>  *in_sig_ptr, in_sig;
   complex<float>  agc_in_sig;
   float power, power_out;
   float input_phase;
   double phase_shift;
   double amplitude;
   double phase_out;
   double sum_in, sum_out;
   double amp_sqrd;
   double avg_power_in, avg_power_out;
   int block_size, is;

   block_size = In_Sig->GetValidBlockSize();
   Out_Sig->SetValidBlockSize(block_size);
   //-------------------------------------------------

   out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
   in_sig_ptr = GET_INPUT_PTR( In_Sig );      
   block_size = In_Sig->GetValidBlockSize();
   Out_Sig->SetValidBlockSize(block_size);
   sum_in = 0.0;
   sum_out = 0.0;

   for(is=0; is<block_size; is++){
      in_sig = *in_sig_ptr++;
      agc_in_sig = in_sig;
      amp_sqrd = std::norm(agc_in_sig);
      sum_in += amp_sqrd;
      power = float(0.5 * Input_Power_Scale_Factor *
                     amp_sqrd);
      if(power == 0.0){
         input_phase = 0.0;
      }
      else{
         input_phase = std::arg(agc_in_sig);
      }
      power_out = float(Output_Power_Scale_Factor *
                     Am_Am_Curve->GetValue(power));
      sum_out += power_out;
      phase_shift = Am_Pm_Curve->GetValue(power);
      amplitude = sqrt(2.0*power_out);

      phase_out = input_phase + 
         Phase_Scale_Factor*phase_shift;

      out_sig = complex<float>(  
            float(amplitude*cos(phase_out)),
            float(amplitude*sin(phase_out)));
      *out_sig_ptr++ = out_sig;
   }
   avg_power_out = sum_out/block_size;
   avg_power_in = sum_in/block_size/2;
   BasicResults 
      << "In NonlinearAmplifier avg input power = " 
      << avg_power_in << endl;
   BasicResults 
      << "                      avg output power = " 
      << avg_power_out << endl;

   return(_MES_AOK);
}

