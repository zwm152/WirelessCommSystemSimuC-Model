//
//  File = phase_rotate_bp.h
//

#ifndef _PHASE_ROTATE_BP_H_
#define _PHASE_ROTATE_BP_H_

#include "signal_T.h"

class BandpassPhaseRotate : public PracSimModel
{
public:
BandpassPhaseRotate( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float> *in_signal,
            Signal<float> *out_signal );

  ~BandpassPhaseRotate(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  float Phase_Shift_Deg;
  std::complex<float> Rotate_Val;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;
  
};

#endif
