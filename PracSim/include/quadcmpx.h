//
//  File = quadcmpx.h
//

#ifndef _QUADCMPX_H_
#define _QUADCMPX_H_

#include "psmodel.h"
#include "signal_T.h"

class QuadratureToComplex : public PracSimModel
{
public:
  QuadratureToComplex( char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* i_in_sig,
                  Signal<float>* q_in_sig,
                  Signal< std::complex<float> >* out_sig);

  ~QuadratureToComplex(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  Signal<float> *I_In_Sig;
  Signal<float> *Q_In_Sig;
  Signal< std::complex<float> > *Out_Sig;
};

#endif
