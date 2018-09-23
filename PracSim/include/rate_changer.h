//
//  File = rate_changer.h
//

#ifndef _RATE_CHANGER_H_
#define _RATE_CHANGER_H_

#include "signal_T.h"

class RateChanger : public PracSimModel
{
public:
RateChanger( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float> *in_signal,
            Signal<float> *out_signal );

  ~RateChanger(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;
  double Rate_Change_Factor;
  int Cumul_Samp_Count;
  int Num_Sidelobes;
  int Num_Save_Samps;
  float *Save_Buffer;
  
};

#endif
