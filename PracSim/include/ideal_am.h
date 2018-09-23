//
//  File = ideal_am.h
//

#ifndef _IDEAL_AM_H_
#define _IDEAL_AM_H_

#include "signal_T.h"

class IdealAmplitudeModulator : public PracSimModel
{
public:
IdealAmplitudeModulator( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float> *carrier_signal,
            Signal<float> *mod_signal,
            Signal<float> *out_sig );

  ~IdealAmplitudeModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Avg_Block_Size;
  int In_Avg_Block_Size;
  Signal<float> *Mod_Sig;
  Signal<float> *Carrier_Sig;
  Signal<float> *Out_Sig;
  double Modulation_Index;
  
};

#endif
