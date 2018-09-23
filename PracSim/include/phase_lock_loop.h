//
//  File = phase_lock_loop.h
//

#ifndef _PHASE_LOCK_LOOP_H_
#define _PHASE_LOCK_LOOP_H_

#include "signal_T.h"

class PhaseLockLoop : public PracSimModel
{
public:
PhaseLockLoop( char* instance_nam,
            PracSimModel *outer_model,
            Signal< std::complex<float> > *in_signal,
            Signal< std::complex<float> > *sqrd_signal_out,
            Signal< std::complex<float> > *out_signal );

  ~PhaseLockLoop(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  float Phase_Shift_Deg;
  std::complex<float> Rotate_Val;
  Signal< std::complex<float> > *In_Sig;
  Signal< std::complex<float> > *Sqrd_Sig_Out;
  Signal< std::complex<float> > *Out_Sig;
  
};

#endif
