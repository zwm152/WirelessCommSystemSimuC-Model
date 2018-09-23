//
//  File = bpskmod.h
//

#ifndef _BPSKMOD_H_
#define _BPSKMOD_H_

#include "signal_T.h"
#include "psmodel.h"

class BpskModulator : public PracSimModel
{
public:
  BpskModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal< std::complex<float> >* cmpx_out_sig );

  ~BpskModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Carrier_Phase_Deg;
  double Carrier_Phase_Rad;
  int Block_Size;
  Signal<float> *In_Sig;
  Signal< std::complex<float> > *Cmpx_Out_Sig;  
};

#endif
