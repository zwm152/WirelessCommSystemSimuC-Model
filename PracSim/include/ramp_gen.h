//
//  File = ramp_gen.h
//

#ifndef _RAMP_GEN_H_
#define _RAMP_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

class RampGener : public PracSimModel
{
public:
  RampGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig );

  ~RampGener(void);
  void Initialize(void);
  int Execute(void);

private:
  //long Initial_Seed;
  //long Seed;
  int Block_Size;
  double Samp_Intvl;

  double Rise_Per_Sample;
  int Samp_Count;

  Signal<float> *Out_Sig;
  
};

#endif //_RAMP_GEN_H_
