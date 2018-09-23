//
//  File = carrier_genie.h
//

#ifndef _CARRIER_GENIE_H_
#define _CARRIER_GENIE_H_

#include "signal_T.h"
#include "psmodel.h"

class CarrierRecoveryGenie : public PracSimModel
{
public:
  CarrierRecoveryGenie( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* Recov_Carrier);

  ~CarrierRecoveryGenie(void);
  void Initialize(void);
  int Execute(void);

private:

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;
  double Samp_Intvl;
  double Carrier_Freq;
  double Carrier_Freq_Rad;
  double Time;

  //--------------------------
  //  Signals

  Signal<float>* Recov_Carrier;

};

#endif
