//
//  File = pwrmtr.h
//

#ifndef _PWRMTR_H_
#define _PWRMTR_H_

#include "psmodel.h"
#include "k_pwrmtr.h"
#include "signal_t.h"

class PowerMeter : public PracSimModel
{
public:
  PowerMeter(  char* instance_nam,
            PracSimModel* outer_model,
            Signal<float>* in_sig,
            Signal<float>* out_sig );

  //============================================
  //  constructor for subordinate instance
  PowerMeter(  char *instance_name,
            PracSimModel *outer_model,
            Signal<float> *in_sig,
            Signal<float> *out_sig,
            float estim_init_input_pwr,
            float time_constant );

  ~PowerMeter(void);
  void Setup(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  Signal<float> *In_Sig;
  Signal<float> *Est_Avg_Sig_Pwr;
  k_PowerMeter<float> *Kernel;

};

#endif
