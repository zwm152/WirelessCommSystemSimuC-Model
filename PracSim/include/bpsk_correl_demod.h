//
//  File = bpskcorreldem.h
//

#ifndef _BPSKCORRELDEM_H_
#define _BPSKCORRELDEM_H_

#include "signal_T.h"
#include "control_T.h"
#include "psmodel.h"

class BpskCorrelationDemod : public PracSimModel
{
public:
  BpskCorrelationDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* in_sig,
                    Signal< std::complex<float> >* phase_ref_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< bit_t >* decis_out );

  ~BpskCorrelationDemod(void);
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
  Signal< bit_t > *Symb_Clock_In;  
  Signal< std::complex<float> > *Phase_Ref_Sig;
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  double Integ_Val;
  std::complex<float> Conj_Ref;
};

#endif
