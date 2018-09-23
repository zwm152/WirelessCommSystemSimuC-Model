//
//  File = k_pwrmtr.h
//

#ifndef _K_PWRMTR_H_
#define _K_PWRMTR_H_

//#include "floatsig.h"

template <class T>
class k_PowerMeter // : public PracSimKernel
{
public:
  k_PowerMeter(  char* instance_nam );

  //============================================
  //  constructor for subordinate instance
  k_PowerMeter(  char *instance_name,
            float estim_init_input_pwr,
            float time_constant );

  ~k_PowerMeter(void);
  //void Setup(void);
  void Initialize(  int max_proc_block_size,
                    double samp_rate);
//  int Execute(void);
  int Execute(  T *in_sig_ptr,
                float *est_avg_sig_pwr,
                int block_size);

private:
  int Max_Proc_Block_Size;
  float Init_Pwr_Output;
  float Integ_Const;
  float *Meas_Pwr_Buf;
  double Const_1;
  double Const_2;
  double Delay_Reg;

};

#endif
