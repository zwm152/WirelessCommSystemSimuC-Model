//
//  File = fsk_2tone_bp.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "fsk_2tone_bp.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
//#include "cmpxsig.h"
extern ParmFile *ParmInput;

//========================================================================
// constructor

FskTwoToneModulator_Bp::FskTwoToneModulator_Bp( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* in_sig,
                              Signal<float>* out_sig)
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(FskTwoToneModulator_Bp);

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Freq_Lo_Hz);
  GET_DOUBLE_PARM(Freq_Hi_Hz);
  GET_FLOAT_PARM(Lo_Control_Val);
  GET_FLOAT_PARM(Hi_Control_Val);
  GET_DOUBLE_PARM(Full_Scale_Output_Level);

  //-------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  //----------------------------------
  //  Compute derived parameters

  Slope_Hi_On = Full_Scale_Output_Level/(Hi_Control_Val - Lo_Control_Val);
  Slope_Lo_On = -Slope_Hi_On;

}
//========================================================
// destructor

FskTwoToneModulator_Bp::~FskTwoToneModulator_Bp( void ){ };


//===========================================

void FskTwoToneModulator_Bp::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  Time = 0.0;
  Cumul_Phase_Lo = 0.0;
  Cumul_Phase_Hi = 0.0;
}

//============================================
int FskTwoToneModulator_Bp::Execute(void)
{
  float *in_sig_ptr, in_val;
  float *out_sig_ptr;
  double cumul_phase_lo, cumul_phase_hi;
  double freq_hi_hz, freq_lo_hz;
  double lo_control_val, hi_control_val;
  double gain_hi, gain_lo;
  double samp_intvl;
  int is;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  //omega_hi = Freq_Hi_Rad;
  freq_lo_hz = Freq_Lo_Hz;
  freq_hi_hz = Freq_Hi_Hz;
  samp_intvl = Samp_Intvl;
  cumul_phase_lo = Cumul_Phase_Lo;
  cumul_phase_hi = Cumul_Phase_Hi;
  lo_control_val = Lo_Control_Val;
  hi_control_val = Hi_Control_Val;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;

    //inst_freq = freq_lo_hz + Hz_Per_Volt * (in_val - Lo_Control_Val);
    gain_hi = Slope_Hi_On * (in_val - Lo_Control_Val);
    gain_lo = Slope_Lo_On * (in_val - Hi_Control_Val);

    cumul_phase_lo = fmod(cumul_phase_lo + samp_intvl * freq_lo_hz, 1.0);
    cumul_phase_hi = fmod(cumul_phase_hi + samp_intvl * freq_hi_hz, 1.0);

    *out_sig_ptr++ = float(gain_lo * sin(TWO_PI * cumul_phase_lo)
                     + gain_hi * sin(TWO_PI * cumul_phase_hi));
    }
  Cumul_Phase_Lo = cumul_phase_lo;
  Cumul_Phase_Hi = cumul_phase_hi;
  return(_MES_AOK);
}

