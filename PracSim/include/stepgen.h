//
//  File = stepgen.h
//

#ifndef _STEPGEN_H_
#define _STEPGEN_H_

#include "psmodel.h"
#include "signal_T.h"

class StepGener : public PracSimModel
{
public:
  StepGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig );

  ~StepGener(void);
  void Initialize(void);
  int Execute(void);

private:
  long Initial_Seed;
  long Seed;
  int Block_Size;
  //double Samp_Rate;
  Signal<float> *Out_Sig;
  
};

#endif //_STEPGEN_H_
