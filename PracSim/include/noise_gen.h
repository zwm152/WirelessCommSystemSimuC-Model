//
//  File = noise_gen.h
//

#ifndef _NOISE_GEN_H_
#define _NOISE_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

class GaussianNoiseGenerator : public PracSimModel
{
public:
  GaussianNoiseGenerator( char* instance_name,
            PracSimModel* outer_model,
            Signal<float>* noise_sig);

  ~GaussianNoiseGenerator(void);
//  void Setup(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  Signal<float> *Noise_Sig;
  int Seed;
  float Noise_Sigma;

};

#endif
