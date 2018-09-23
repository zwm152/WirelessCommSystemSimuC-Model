//
//  File = quaddem.h
//

#ifndef _QUADDEM_H_
#define _QUADDEM_H_

#include "signal_T.h"
#include "psmodel.h"

class QuadratureDemod : public PracSimModel
{
public:
  QuadratureDemod(  char* instance_name,
                    PracSimModel* outer_model,
                    Signal< std::complex< float > >* in_sig,
                    Signal< std::complex< float > >* carrier_ref_sig,
                    Signal< float >* i_wave_out,
                    Signal< float >* q_wave_out );
  ~QuadratureDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Amp_Unbal;
  double Real_Unbal;
  double Imag_Unbal;
  int Block_Size;
  bool Polar_Outputs_Enabled;
  Signal<float> *I_Wave_Out;
  Signal<float> *Q_Wave_Out;
  Signal< std::complex<float> > *Carrier_Ref_Sig;  
  Signal< std::complex<float> > *In_Sig;  
};

#endif
