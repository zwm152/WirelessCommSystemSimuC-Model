//
//  File = tone_gen.h
//

#ifndef _TONE_GEN_H_
#define _TONE_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

class ToneGener : public PracSimModel
{
public:
  ToneGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig );

  ~ToneGener(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  double Samp_Intvl;

  double Freq_In_Hz;
  double Arg_Increm;
  double Base_Arg;
  double Phase_In_Deg;
  double Phase_In_Cycles;

  Signal<float> *Out_Sig;
  
};

#endif //_TONE_GEN_H_
