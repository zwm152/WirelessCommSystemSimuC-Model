//
//  File = nonlinear_amp.h
//

#ifndef _NONLINEAR_AMP_H_
#define _NONLINEAR_AMP_H_

#include "signal_T.h"
#include "samp_curve.h"
using std::complex;

class NonlinearAmplifier : public PracSimModel
{
public:
NonlinearAmplifier( char* instance_nam,
            PracSimModel *outer_model,
            Signal< complex<float> > *in_signal,
            Signal< complex<float> > *out_sig );

  ~NonlinearAmplifier(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Avg_Block_Size;
  int In_Avg_Block_Size;
  Signal< complex<float> > *In_Sig;
  Signal< complex<float> > *Out_Sig;
  double Output_Power_Scale_Factor;
  double Phase_Scale_Factor;
  double Anticipated_Input_Power;
  double Operating_Point;
  double Agc_Time_Constant;
  float Input_Power_Scale_Factor;
  bool Agc_On;
  SampledCurve *Am_Am_Curve;
  SampledCurve *Am_Pm_Curve;
  char *Am_Am_Fname;
  char *Am_Pm_Fname;
  
};

#endif
