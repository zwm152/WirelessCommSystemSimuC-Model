//
//  File = qamoptimdem.h
//

#ifndef _QAMOPTIMDEM_H_
#define _QAMOPTIMDEM_H_

#include "signal_T.h"
#include "psmodel.h"

class QamOptimalDemod : public PracSimModel
{
public:
  QamOptimalDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* in_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< byte_t >* out_sig );

  ~QamOptimalDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Dly_To_Start;
  double Out_Samp_Intvl;
  int Block_Size;
  Signal< byte_t > *Out_Sig;
  Signal< std::complex<float> > *In_Sig;  
  Signal< bit_t > *Symb_Clock_In;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  int Num_Symb_Rows;
  double *I_Boundary;
  double *Q_Boundary;
  double I_Integ_Val;
  double Q_Integ_Val;
};

#endif
