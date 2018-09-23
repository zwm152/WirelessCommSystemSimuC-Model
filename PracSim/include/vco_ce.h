//
//  File = vco_ce.h
//

#ifndef _VCO_BP_H_
#define _VCO_BP_H_

#include "signal_T.h"
#include "psmodel.h"
using std::complex;

class ComplexVco : public PracSimModel
{
public:
  ComplexVco(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal< complex<float> >* out_sig );

  ~ComplexVco(void);
  void Initialize(void);
  int Execute(void);

private:
  double Freq_Hi_Hz;
  double Freq_Lo_Hz;
  float Lo_Control_Val;
  float Hi_Control_Val;
  double Hz_Per_Volt;
  int Block_Size;
  double Samp_Intvl;
  double Cumul_Phase;
  double Time;
  Signal<float> *In_Sig;
  Signal< complex<float> > *Out_Sig;  
};

#endif
