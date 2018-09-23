//
//  File = fsk_genie.h
//

#ifndef _FSK_GENIE_H_
#define _FSK_GENIE_H_

#include "signal_T.h"
#include "psmodel.h"

class FskCarrierGenie : public PracSimModel
{
public:

  FskCarrierGenie(  char* instance_name,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> > *lo_ref_sig,
                    Signal< std::complex<float> > *hi_ref_sig );

  ~FskCarrierGenie(void);
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

  Signal< std::complex<float> >* Lo_Ref_Sig;
  Signal< std::complex<float> >* Hi_Ref_Sig;

};

#endif
