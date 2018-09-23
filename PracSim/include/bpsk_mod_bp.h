//
//  File = bpsk_mod_bp.h
//

#ifndef _BPSK_MOD_BP_H_
#define _BPSK_MOD_BP_H_

#include "signal_T.h"
#include "psmodel.h"

class BpskBandpassModulator : public PracSimModel
{
public:
  BpskBandpassModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal<float>* out_sig );

  ~BpskBandpassModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Carrier_Freq;
  double Carrier_Freq_Rad;
  int Block_Size;
  double Samp_Intvl;
  double Time;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;  
};

#endif
