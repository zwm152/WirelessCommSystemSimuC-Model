//
//  File = sin_awgn.h
//

#ifndef _SIN_AWGN_H_
#define _SIN_AWGN_H_

#include "psmodel.h"
#include "signal_T.h"

class SinesInAwgn : public PracSimModel
{
public:
  SinesInAwgn( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig );

  ~SinesInAwgn(void);
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
  long Agn_Initial_Seed;
  long Agn_Seed;
  double Agn_Sigma;

  Signal<float> *Out_Sig;
  
};

#endif
