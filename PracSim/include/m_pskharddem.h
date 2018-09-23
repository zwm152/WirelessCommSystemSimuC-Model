//
//  File = m_pskharddem.h
//

#ifndef _M_PSKHARDDEM_H_
#define _M_PSKHARDDEM_H_

#include "signal_T.h"
#include "psmodel.h"

class M_PskHardDemod : public PracSimModel
{
public:
  M_PskHardDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* in_sig,
                    Signal< std::complex<float> >* integ_sig,
                    Signal< byte_t >* out_sig );

  ~M_PskHardDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Dly_To_Start;
  double Out_Samp_Intvl;
  int Block_Size;
  Signal< byte_t > *Out_Sig;
  Signal< std::complex<float> > *In_Sig;  
  Signal< std::complex<float> > *Integ_Sig;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  int Num_Diff_Symbs;
  float *Decis_Bound;
  std::complex<float> Integ_Val;
};

#endif
