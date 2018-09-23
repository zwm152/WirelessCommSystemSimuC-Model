//
//  File = phase_genie.h
//

#ifndef _PHASE_GENIE_H_
#define _PHASE_GENIE_H_

#include "signal_T.h"
#include "psmodel.h"

class PhaseRecoveryGenie : public PracSimModel
{
public:
  PhaseRecoveryGenie( char* instance_name,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* recov_carrier_sig);

  ~PhaseRecoveryGenie(void);
  void Initialize(void);
  int Execute(void);

private:

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;
  double Samp_Intvl;
  double Carrier_Phase_Deg;
  double Carrier_Phase;
  std::complex<float> Carrier_Val;

  //--------------------------
  //  Signals

  Signal< std::complex<float> > *Recov_Carrier_Sig;
};

#endif
