//
//  File = linear_pll.h
//

#ifndef _LINEAR_PLL_H_
#define _LINEAR_PLL_H_

#include "signal_T.h"
#include "anlg_filt_iir.h"

class LinearPLL : public PracSimModel
{
public:
LinearPLL( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float> *in_signal,
            Signal<float> *err_signal,
            Signal<float> *filt_err_signal,
            Signal<float> *ref_signal,
            Signal<float> *vco_freq_sig,
            Signal<float> *ref_phase_sig,
            Signal<float> *out_signal );

   ~LinearPLL(void);
   void Initialize(void);
   int Execute(void);

private:
   double Samp_Intvl;

   Signal<float> *fsig_Input;
   Signal<float> *fsig_Phase_Error;
   Signal<float> *fsig_Filtered_Error;
   Signal<float> *fsig_Osc_Output;
   Signal<float> *fsig_Osc_Freq;
   Signal<float> *fsig_Osc_Phase;
   Signal<float> *fsig_Output;

   bool m_UsingDco;
   double K_Sub_D;
   double K_Sub_0;
   double OscOutput;
   double Phi_Sub_2;
   double Phi_Divided;
   double Omega_Sub_0;
   double Center_Freq_Hz;
   float Osc_Output_Prev_Val;
   double Scaler_Divisor;
  AnalogFilterByIir<float> *Filter_Core;
  
};

#endif
