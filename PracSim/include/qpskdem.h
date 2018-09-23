//
//  File = qpskdem.h
//

#ifndef _QPSKDEM_H_
#define _QPSKDEM_H_

#include "signal_T.h"
#include "psmodel.h"

class QpskDemodulator : public PracSimModel
{
public:
  QpskDemodulator(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< std::complex<float> >* in_sig,
                    Signal< float >* out_sig_i,
                    Signal< float >* out_sig_q );

  ~QpskDemodulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  int Block_Size;
  Signal< float > *I_Out_Sig;
  Signal< float > *Q_Out_Sig;
  Signal< std::complex<float> > *In_Sig;  
};

#endif
