//
//  File = qpskoptbitdem.h
//

#ifndef _QPSKOPTBITDEM_H_
#define _QPSKOPTBITDEM_H_

#include "signal_T.h"
#include "psmodel.h"

class QpskOptimalBitDemod : public PracSimModel
{
public:
  QpskOptimalBitDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* in_sig,
                    Signal< std::complex< float > >* carrier_ref_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< bit_t >* i_decis_out,
                    Signal< bit_t >* q_decis_out );

  ~QpskOptimalBitDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Out_Samp_Intvl;
  int Block_Size;
  Signal< bit_t > *I_Decis_Out;
  Signal< bit_t > *Q_Decis_Out;
  Signal< std::complex<float> > *In_Sig;  
  Signal< std::complex< float > > *Carrier_Ref_Sig;
  Signal< bit_t > *Symb_Clock_In;  
  int Samps_Per_Symb;
  bool Constel_Offset_Enabled;
  double *Integ_Val;
  std::complex<float> Constel_Offset_Rot;

};

#endif
