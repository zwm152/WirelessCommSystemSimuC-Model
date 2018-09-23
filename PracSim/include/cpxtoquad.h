//
//  File = cpxtoquad.h
//

#ifndef _CPXTOQUAD_H_
#define _CPXTOQUAD_H_

#include "signal_T.h"
#include "psmodel.h"

class CmpxToQuadrature : public PracSimModel
{
public:
  CmpxToQuadrature( char* instance_nam,
               PracSimModel* outer_model,
               Signal< std::complex<float> >* in_signal,
               Signal<float>* i_out_signal,
               Signal<float>* q_out_signal);

  ~CmpxToQuadrature(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal< std::complex<float> > *In_Sig;
  Signal<float> *I_Out_Sig;
  Signal<float> *Q_Out_Sig;
};

#endif
