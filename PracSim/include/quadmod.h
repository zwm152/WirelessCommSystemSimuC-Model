//
//  File = quadmod.h
//

#ifndef _QUADMOD_H_
#define _QUADMOD_H_

#include "signal_T.h"
#include "psmodel.h"

class QuadratureModulator : public PracSimModel
{
public:
  QuadratureModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* i_in_sig,
                  Signal<float>* q_in_sig,
                  Signal< std::complex<float> >* cmpx_out_sig,
                  Signal<float>* mag_out_sig,
                  Signal<float>* phase_out_sig );
  QuadratureModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* i_in_sig,
                  Signal<float>* q_in_sig,
                  Signal< std::complex<float> >* cmpx_out_sig );
  void Constructor_Common_Tasks( char* instance_name,
                            Signal<float>* i_in_sig,
                            Signal<float>* q_in_sig,
                            Signal< std::complex<float> >* cmpx_out_sig);

  ~QuadratureModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Amp_Unbal;
  double Real_Unbal;
  double Imag_Unbal;
  int Block_Size;
  bool Polar_Outputs_Enabled;
  Signal<float> *I_In_Sig;
  Signal<float> *Q_In_Sig;
  Signal< std::complex<float> > *Cmpx_Out_Sig;  
  Signal<float> *Mag_Out_Sig;
  Signal<float> *Phase_Out_Sig;
};

#endif
