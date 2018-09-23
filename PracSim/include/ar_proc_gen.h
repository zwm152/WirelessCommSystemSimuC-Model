//
//  File = ar_proc_gen.h
//

#ifndef _AR_PROC_GEN_H_
#define _AR_PROC_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

template < class T>
class ArProcessGenerator : public PracSimModel
{
public:
  ArProcessGenerator( char* instance_name,
            PracSimModel* outer_model,
            Signal<T>* noise_sig);

  ~ArProcessGenerator(void);
//  void Setup(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  Signal<T> *Noise_Sig;
  int Noise_Seed;
  float Driving_Sigma;
  double Driving_Variance;
  T *Old_Output;
  double *A_Coeffs;
  int Ar_Order;

};

#endif
