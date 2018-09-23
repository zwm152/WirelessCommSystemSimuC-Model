//
//  File = cmpx_multi_sine_gen.h
//

#ifndef _CMPX_MULTI_SINE_GEN_H_
#define _CMPX_MULTI_SINE_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

class CmpxMultipleSineGen : public PracSimModel
{
public:
  CmpxMultipleSineGen( char* instance_nam,
            PracSimModel *outer_model,
            Signal< std::complex<float> >* out_sig );

  ~CmpxMultipleSineGen(void);
  void Initialize(void);
  int Execute(void);

private:
  //long Initial_Seed;
  //long Seed;
  int Block_Size;
  double Samp_Intvl;

  int Num_Sines;

  double *Freq_In_Hz;
  double *Arg_Increm;
  double *Base_Arg;
  double *Phase_In_Deg;
  double *Phase_In_Rad;
  double *Tone_Gain;

  Signal< std::complex<float> > *Out_Sig;
  
};

#endif //_CMPX_MULTI_SINE_GEN_H_
