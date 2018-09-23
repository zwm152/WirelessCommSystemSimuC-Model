//
//  File = fsk_demod_bp.h
//

#ifndef _FSK_BP_DEMOD_H_
#define _FSK_BP_DEMOD_H_

#include "signal_T.h"
#include "psmodel.h"
#include "butt_filt_iir.h"

class FskBandpassDemod : public PracSimModel
{
public:
  FskBandpassDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< float >* in_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< bit_t >* decis_out,
                    Signal< float >* lo_filt_output,
                    Signal< float >* hi_filt_output,
                    Signal< float >* lo_integ_sig,
                    Signal< float >* hi_integ_sig );

  ~FskBandpassDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Dly_To_Start;
  double Out_Samp_Intvl;
  double In_Samp_Intvl;
  //double Carrier_Phase_Deg;
  //double Carrier_Phase_Rad;
  int Block_Size;
  Signal< bit_t > *Decis_Out;
  Signal< float > *In_Sig;  
  Signal< float > *Lo_Filt_Output;  
  Signal< float > *Hi_Filt_Output;  
  Signal< float > *Lo_Integ_Sig;  
  Signal< float > *Hi_Integ_Sig;  
  Signal< bit_t > *Symb_Clock_In;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  double Lo_Integ_Val;
  double Hi_Integ_Val;
  ButterworthFilterByIir<float> *Lo_Freq_Filter;
  ButterworthFilterByIir<float> *Hi_Freq_Filter;
  ButterworthFilterByIir<float> *Lo_Envel_Filter;
  ButterworthFilterByIir<float> *Hi_Envel_Filter;
  std::complex<float> Conj_Ref;
};

#endif
