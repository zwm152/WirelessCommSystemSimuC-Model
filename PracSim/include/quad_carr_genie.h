//
//  File = quad_carr_genie.h
//

#ifndef _QUAD_CARR_GENIE_H_
#define _QUAD_CARR_GENIE_H_

#include "signal_T.h"
#include "psmodel.h"

class QuadCarrierRecovGenie : public PracSimModel
{
public:
  QuadCarrierRecovGenie( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* I_Recov_Carrier,
                    Signal<float>* Q_Recov_Carrier);

  ~QuadCarrierRecovGenie(void);
  void Initialize(void);
  int Execute(void);

private:

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;
  double Samp_Intvl;
  double Carrier_Freq;
  double Carrier_Freq_Rad;
  double Phase_Offset_Deg;
  double Phase_Offset_Rad;
  double Time;

  //--------------------------
  //  Signals

  Signal<float>* I_Recov_Carrier;
  Signal<float>* Q_Recov_Carrier;

};

#endif
