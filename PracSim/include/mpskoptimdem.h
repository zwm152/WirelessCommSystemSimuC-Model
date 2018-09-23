//
//  File = m_pskoptimdem.h
//

#ifndef _M_PSKOPTIMDEM_H_
#define _M_PSKOPTIMDEM_H_

#include "signal_T.h"
#include "psmodel.h"

class MpskOptimalDemod : public PracSimModel
{
public:
  MpskOptimalDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< complex<float> >* in_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< byte_t >* out_sig );

  ~MpskOptimalDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Out_Samp_Intvl;
  int Block_Size;
  Signal< byte_t > *Out_Sig;
  Signal< std::complex<float> > *In_Sig;  
  Signal< bit_t > *Symb_Clock_In;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  byte_t Num_Diff_Symbs;
  double *Integ_Val;
  std::complex<float> *Conj_Ref;
};

#endif
