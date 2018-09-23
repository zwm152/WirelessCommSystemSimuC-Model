//
//  File = qpskmod.h
//

#ifndef _QPSKMOD_H_
#define _QPSKMOD_H_

#include "signal_T.h"
#include "psmodel.h"

class QpskModulator : public PracSimModel
{
public:
  QpskModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* i_in_sig,
                  Signal<float>* q_in_sig,
                  Signal< std::complex<float> >* cmpx_out_sig,
                  Signal<float>* mag_out_sig,
                  Signal<float>* phase_out_sig );

  ~QpskModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Amp_Unbal;
  double Real_Unbal;
  double Imag_Unbal;
  int Block_Size;
  Signal<float> *I_In_Sig;
  Signal<float> *Q_In_Sig;
  Signal< std::complex<float> > *Cmpx_Out_Sig;  
  Signal<float> *Mag_Out_Sig;
  Signal<float> *Phase_Out_Sig;
};

#endif
