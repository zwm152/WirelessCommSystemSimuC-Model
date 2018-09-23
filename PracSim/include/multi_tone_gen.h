//
//  File = multi_tone_gen.h
//

#ifndef _MULTI_TONE_GEN_H_
#define _MULTI_TONE_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

class MultipleToneGener : public PracSimModel
{
public:
  MultipleToneGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig );

  ~MultipleToneGener(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  double Samp_Intvl;
  int Num_Sines;
  double Holdoff_Delay;
  double *Freq_In_Hz;
  double *Arg_Increm;
  double *Base_Arg;
  double *Phase_In_Deg;
  double *Phase_In_Rad;
  double *Tone_Gain;

  Signal<float> *Out_Sig;
  
};
#endif //_MULTI_TONE_GEN_H_