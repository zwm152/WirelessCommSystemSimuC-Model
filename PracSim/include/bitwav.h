//
//  File = bitwav.h
//

#ifndef _BITWAV_H_
#define _BITWAV_H_

#include "signal_T.h"

class BitsToWave : public PracSimModel
{
public:
BitsToWave( char* instance_nam,
            PracSimModel *outer_model,
            Signal<bit_t> *in_signal,
            Signal<float> *out_signal,
            Signal<bit_t> *bit_clock_out );

  ~BitsToWave(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Avg_Block_Size;
  int In_Avg_Block_Size;
  Signal<bit_t> *In_Sig;
  Signal<float> *Out_Sig;
  Signal<bit_t> *Bit_Clock_Out;
  double Pulse_Duration;
  double Delay_To_First_Edge;
  double Lo_Val;
  double Hi_Val;
  float Last_Bit_Val;
  double Samps_In_Curr_Bit;
  float Samps_Per_Bit;
  
};

#endif
