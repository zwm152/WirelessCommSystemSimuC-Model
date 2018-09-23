//
//  File = digital_pll.h
//

#ifndef _DIGITAL_PLL_H_
#define _DIGITAL_PLL_H_

#include "signal_T.h"
#include "anlg_filt_iir.h"

class DigitalPLL : public PracSimModel
{
public:
   DigitalPLL( char* instance_nam,
               PracSimModel *outer_model,
               Signal<float> *in_signal,
               Signal<float> *filt_err_signal,
               Signal<float> *ref_signal,
               Signal<float> *vco_freq_sig,
               Signal<float> *ref_phase_sig,
               Signal<float> *out_signal );
   ~DigitalPLL(void);
   void Initialize(void);
   int Execute(void);

private:
   double Samp_Intvl;
   Signal<float> *fsig_Input;
   Signal<float> *fsig_Filtered_Error;
   Signal<float> *fsig_Osc_Output;
   Signal<float> *fsig_Osc_Freq;
   Signal<float> *fsig_Osc_Phase;
   Signal<float> *fsig_Output;
   bool m_UsingDco;
   bool Prev_Input_Positive;
   double Time_Of_Samp;
   double Prev_Filt_Val;
   double Prev_Cap_Val;
   double Supply_Volts;
   double Prev_Time_Zc;
   double Prev_Osc_Phase;
   int Prev_State;
   double Tau_1, Tau_2;
   float Prev_Input_Val;
   double K_Sub_0, K_Sub_D;
   double OscOutput;
   double Phi_Sub_2;
   double Omega_Sub_0;
   double Center_Freq_Hz;
   float Osc_Output_Prev_Val;
   AnalogFilterByIir<float> *Filter_Core;
};

#endif
