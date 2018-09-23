//
//  File = vco_ce.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "vco_ce.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
extern ParmFile *ParmInput;
//using std::complex;

//========================================================================
// constructor

ComplexVco::ComplexVco( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* in_sig,
                              Signal< complex<float> >* out_sig)
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(ComplexVco);

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Freq_Lo_Hz);
  GET_DOUBLE_PARM(Freq_Hi_Hz);
  GET_FLOAT_PARM(Lo_Control_Val);
  GET_FLOAT_PARM(Hi_Control_Val);

  //-------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  //----------------------------------
  //  Compute derived parameters

  Hz_Per_Volt = (Freq_Hi_Hz - Freq_Lo_Hz)/(Hi_Control_Val - Lo_Control_Val);

}
//========================================================
// destructor

ComplexVco::~ComplexVco( void ){ };


//===========================================

void ComplexVco::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  Time = 0.0;
  Cumul_Phase = 0.0;
}

//============================================
int ComplexVco::Execute(void)
{
  float *in_sig_ptr, in_val;
  complex<float> *out_sig_ptr;
  double inst_freq, freq_lo_hz;
  double cumul_phase;
  double samp_intvl;
  int is;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  freq_lo_hz = Freq_Lo_Hz;
  samp_intvl = Samp_Intvl;
  cumul_phase = Cumul_Phase;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;

    inst_freq = freq_lo_hz + Hz_Per_Volt * (in_val - Lo_Control_Val);

    cumul_phase = fmod(cumul_phase + samp_intvl * inst_freq, 1.0);
    *out_sig_ptr++ = complex<float>( float(cos(TWO_PI * cumul_phase)),
                                     float(sin(TWO_PI * cumul_phase)));
    }
  Cumul_Phase = cumul_phase;
  return(_MES_AOK);
}

