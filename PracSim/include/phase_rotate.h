//
//  File = phase_rotate.h
//

#ifndef _PHASE_ROTATE_H_
#define _PHASE_ROTATE_H_

#include "signal_T.h"

class PhaseRotate : public PracSimModel
{
public:
PhaseRotate( char* instance_nam,
            PracSimModel *outer_model,
            Signal< std::complex<float> > *in_signal,
            Signal< std::complex<float> > *out_signal );

  ~PhaseRotate(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  float Phase_Shift_Deg;
  std::complex<float> Rotate_Val;
  Signal< std::complex<float> > *In_Sig;
  Signal< std::complex<float> > *Out_Sig;
  
};

#endif
