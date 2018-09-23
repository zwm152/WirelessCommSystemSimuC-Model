//
//  File = ideal_hard_lim.h
//

#ifndef _IDEAL_HARD_LIM_H_
#define _IDEAL_HARD_LIM_H_

#include "signal_T.h"

class IdealHardLimiter : public PracSimModel
{
public:
IdealHardLimiter( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float> *in_signal,
            Signal<float> *out_sig );

  ~IdealHardLimiter(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Avg_Block_Size;
  int In_Avg_Block_Size;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;
  double Pos_Limit;
  double Neg_Limit;
  
};

#endif
