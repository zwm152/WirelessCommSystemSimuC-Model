//
//  File = clockgen.h
//

#ifndef _CLOCKGEN_H_
#define _CLOCKGEN_H_

#include "signal_T.h"

class ClockGener : public PracSimModel
{
public:
ClockGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<bit_t> *out_signal );

  ~ClockGener(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Block_Size;
  int In_Block_Size;
  Signal<bit_t> *Out_Sig;
  double Pulse_Duration;
  double Delay_To_First_Edge;
  double Samps_In_Curr_Bit;
  float Samps_Per_Bit;
  
};

#endif
