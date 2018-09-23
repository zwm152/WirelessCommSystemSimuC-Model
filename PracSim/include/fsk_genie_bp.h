//
//  File = fsk_genie_bp.h
//

#ifndef _FSK_GENIE_H_
#define _FSK_GENIE_H_

#include "signal_T.h"
#include "psmodel.h"

class FskBandpassCarrierGenie : public PracSimModel
{
public:

  FskBandpassCarrierGenie(  char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float> *lo_ref_sig,
                    Signal<float> *hi_ref_sig );

  ~FskBandpassCarrierGenie(void);
  void Initialize(void);
  int Execute(void);

private:

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;
  double Samp_Intvl;
  double Carrier_Freq_Hz;
  double Freq_Offset_Hz;
  double Cumul_Phase_Lo;
  double Cumul_Phase_Hi;
  double Freq_Hi_Hz;
  double Freq_Lo_Hz;
  int Lo_Ref_Samp_Shift;
  int Hi_Ref_Samp_Shift;


  //--------------------------
  //  Signals

  Signal<float>* Lo_Ref_Sig;
  Signal<float>* Hi_Ref_Sig;

};

#endif
