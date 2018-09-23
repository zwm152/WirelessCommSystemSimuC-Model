//
//  File = basewav.h
//

#ifndef _BASEWAV_H_
#define _BASEWAV_H_

#include "signal_T.h"
#include "wave_kinds.h"

class BasebandWaveform : public PracSimModel
{
public:
BasebandWaveform( char* instance_nam,
            PracSimModel *outer_model,
            Signal<bit_t> *in_signal,
            Signal<float> *out_signal,
            Signal<bit_t> *bit_clock_out );

  ~BasebandWaveform(void);
  void Initialize(void);
  int Execute(void);

private:
   PCM_WAVE_KIND_T Pcm_Wave_Kind;
  int Out_Block_Size;
  int In_Block_Size;
  Signal<bit_t> *In_Sig;
  Signal<float> *Out_Sig;
  Signal<bit_t> *Bit_Clock_Out;
  double Bit_Duration;
  double Delay_To_First_Edge;
  float Lo_Lev;
  float Hi_Lev;
  float Init_Lev;
  float Curr_Lev;
  float Last_Bit_Val;
  double Samps_In_Curr_Bit;
  float Samps_Per_Bit;
  float Samps_Per_Half_Bit;  
  bool Bit_Clock_Enab;
  bool In_First_Half;
  bool In_Startup_Delay;
  bool Transit_At_End;
  bool Transit_In_Center;
  bool Init_Lev_Is_Lo;
};
#endif