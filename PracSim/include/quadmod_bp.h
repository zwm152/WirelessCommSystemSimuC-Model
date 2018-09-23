//
//  File = quadmod_bp.h
//

#ifndef _QUADMOD_BP_H_
#define _QUADMOD_BP_H_

#include "signal_T.h"
#include "psmodel.h"

class QuadBandpassModulator : public PracSimModel
{
public:
  QuadBandpassModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* i_in_sig,
                  Signal<float>* q_in_sig,
                  Signal<float>* out_sig);

  ~QuadBandpassModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Phase_Unbal_Rad;
  double Amp_Unbal;
  double Carrier_Freq;
  double Carrier_Freq_Rad;
  double Phase_Offset_Deg;
  double Phase_Offset_Rad;
  double Time;
  int Block_Size;
  double Samp_Intvl;
  Signal<float> *I_In_Sig;
  Signal<float> *Q_In_Sig;
  Signal<float> *Out_Sig;  
};

#endif
