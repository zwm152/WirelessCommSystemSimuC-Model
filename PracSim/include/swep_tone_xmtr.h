//
//  File = swep_tone_xmtr.h
//

#ifndef _SWEP_TONE_XMTR_H_
#define _SWEP_TONE_XMTR_H_

#include "psmodel.h"
#include "signal_T.h"
#include "control_T.h"

class SweptToneTransmitter : public PracSimModel
{
public:
  SweptToneTransmitter( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig,
            Control<bool>* rx_enable,
            Control<double>* inst_freq_ctl,
            Control<double>* block_phase_ctl );

  ~SweptToneTransmitter(void);
  void Initialize(void);
  int Execute(void);

private:
  //long Initial_Seed;
  //long Seed;
  int Block_Size;
  double Samp_Intvl;

  double Beg_Freq_In_Hz;
  double End_Freq_In_Hz;
  int Num_Freqs;
  int Num_Holdoff_Passes;

  double Phase_Increm;
  double Base_Phase;

  Signal<float> *Out_Sig;
  Control<double> *Inst_Freq_Ctl;
  Control<double> *Block_Phase_Ctl;
  Control<bool> *Rx_Enable;
  
};

#endif //_SWEP_TONE_XMTR_H_
