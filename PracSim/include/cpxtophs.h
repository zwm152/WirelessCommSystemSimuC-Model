//
//  File = cpxtophs.h
//

#ifndef _CPXTOPHS_H_
#define _CPXTOPHS_H_

#include "signal_T.h"
#include "psmodel.h"

class CmpxToPhase : public PracSimModel
{
public:
  CmpxToPhase( char* instance_nam,
               PracSimModel* outer_model,
               Signal< std::complex<float> >* in_signal,
               Signal<float>* out_signal,
               Signal<float>* mag_signal);

  ~CmpxToPhase(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal< std::complex<float> > *In_Sig;
  Signal<float> *Out_Sig;
  Signal<float> *Mag_Sig;
};

#endif
