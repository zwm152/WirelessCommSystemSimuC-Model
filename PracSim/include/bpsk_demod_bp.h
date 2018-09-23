//
//  File = bpsk_demod_bp.h
//

#ifndef _BPSK_BP_DEMOD_H_
#define _BPSK_BP_DEMOD_H_

#include "signal_T.h"
#include "psmodel.h"

class BpskBandpassDemod : public PracSimModel
{
public:
  BpskBandpassDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< float >* in_sig,
                    Signal< float >* ref_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< bit_t >* decis_out );

  ~BpskBandpassDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Dly_To_Start;
  double Out_Samp_Intvl;
  //double Carrier_Phase_Deg;
  //double Carrier_Phase_Rad;
  int Block_Size;
  Signal< bit_t > *Decis_Out;
  Signal< float > *In_Sig;  
  Signal< float > *Ref_Sig;  
  Signal< bit_t > *Symb_Clock_In;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  double Integ_Val;
  std::complex<float> Conj_Ref;
};

#endif
