//
//  File = noise_gen.h
//

#ifndef _RAYL_GEN_H_
#define _RAYL_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

template < class T>
class RayleighNoiseGenerator : public PracSimModel
{
public:
  RayleighNoiseGenerator( char* instance_name,
            PracSimModel* outer_model,
            Signal<T>* noise_sig);

  ~RayleighNoiseGenerator(void);
//  void Setup(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  Signal<T> *Noise_Sig;
  int Seed;
  float Noise_Sigma;

};

#endif
