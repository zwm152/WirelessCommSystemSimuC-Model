//
//  File = fsk_demod_coh.h
//

#ifndef _FSK_DEMOD_COH_H_
#define _FSK_DEMOD_COH_H_

#include "signal_T.h"
#include "psmodel.h"

class FskCoherentDemod : public PracSimModel
{
public:
  FskCoherentDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* in_sig,
                    Signal< std::complex<float> >* lo_ref_sig,
                    Signal< std::complex<float> >* hi_ref_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< bit_t >* decis_out,
                    Signal< float >* lo_integ_sig,
                    Signal< float >* hi_integ_sig );

  ~FskCoherentDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Dly_To_Start;
  double Out_Samp_Intvl;
  //double Carrier_Phase_Deg;
  //double Carrier_Phase_Rad;
  int Block_Size;
  Signal< bit_t > *Decis_Out;
  Signal< std::complex<float> > *In_Sig;  
  Signal< std::complex<float> > *Lo_Ref_Sig;  
  Signal< std::complex<float> > *Hi_Ref_Sig;  
  Signal< float > *Lo_Integ_Sig;  
  Signal< float > *Hi_Integ_Sig;  
  Signal< bit_t > *Symb_Clock_In;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  double Lo_Integ_Val;
  double Hi_Integ_Val;
  std::complex<float> Conj_Ref;
};

#endif
