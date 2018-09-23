//
//  File = add_gaus_noise.h
//

#ifndef _ADD_GAUS_NOISE_H_
#define _ADD_GAUS_NOISE_H_

#include "psmodel.h"
#include "signal_T.h"
#include "k_pwrmtr.h"
using std::complex;

template < class T>
class AdditiveGaussianNoise : public PracSimModel
{
public:

   AdditiveGaussianNoise( char* instance_name,
            PracSimModel* outer_model,
            Signal<T>* in_sig,
            Signal<T>* sig_with_noise,
            Signal<float>* measured_power_sig);

   AdditiveGaussianNoise( char* instance_name,
            PracSimModel* outer_model,
            Signal<T>* in_sig,
            Signal<T>* sig_with_noise,
            Signal<T>* noise_only,
            Signal<float>* measured_power_sig);

   ~AdditiveGaussianNoise(void);
   void Initialize(void);
   int Execute(void);

private:
   int Proc_Block_Size;
   double Samp_Rate;
   Signal<T> *In_Sig;
   Signal<T> *Noisy_Sig;
   Signal<T> *Noise_Only_Sig;
   Signal<float> *Power_Meas_Sig;
   float *Sig_Pwr_Sig;
   float Desired_Avg_Pwr;
   float Estim_Init_Input_Pwr;
   float Sqrt_Powers;
   k_PowerMeter<T> *Power_Meter; 
   float Anticip_Input_Pwr;
   float Desired_Output_Pwr;
   float Desired_Eb_No;
   float Symb_Period;
   float Num_Bits_Per_Symb;
   float Time_Const_For_Pwr_Mtr;
   int Seed;
   bool Sig_Pwr_Meas_Enabled;
   bool Outpt_Pwr_Scaling_On;
   float Noise_Sigma;
   float Power_Scaler;

   std::complex<double> Sum;
   double Sum_Sqrd;
   int Num_Samps;
   double Cumul_Batch_Power;
};

#endif
