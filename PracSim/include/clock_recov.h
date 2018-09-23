//
//  File = clock_recov.h
//

#ifndef _CLOCK_RECOV_H_
#define _CLOCK_RECOV_H_

#include "signal_T.h"
#include "anlg_filt_iir.h"

class ClockRecoveryLoop : public PracSimModel
{
public:
ClockRecoveryLoop( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float> *in_signal,
            Signal<float> *raw_err_signal,
            Signal<float> *filt_err_signal,
            Signal<float> *fsig_symbol_period,
            Signal<float> *out_signal );

   ~ClockRecoveryLoop(void);
   void Initialize(void);
   int Execute(void);

private:
   double Samp_Intvl;

   Signal<float> *fsig_Input;
  // Signal<float> *fsig_Phase_Error;
   Signal<float> *fsig_Filtered_Error;
   Signal<float> *fsig_Raw_Error;
  // Signal<float> *fsig_Osc_Output;
  // Signal<float> *fsig_Osc_Freq;
  // Signal<float> *fsig_Osc_Phase;
   Signal<float> *fsig_Output;
   Signal<float> *fsig_Symbol_Period;

   double K_Sub_0;
  // double I_OscOutput;
  // double Q_OscOutput;
  // double Phi_Sub_2;
  // double Phi_Divided;
   double Omega_Sub_0;
   double Center_Freq_Hz;
  // float Osc_Output_Prev_Val;
 //  double Scaler_Divisor;
   double Reduced_Time;
   double Gate_Fraction;
   double Nominal_Symbol_Period;
   double Adjusted_Symbol_Period;
   double Early_Gate_Sum;
   double Early_Gate_Latched_Sum;
   double Late_Gate_Sum;
   double Late_Gate_Latched_Sum;
   double Gate_Diff;
   double Early_Gate_Open_Time;
   double Early_Gate_Close_Time;
   double Late_Gate_Open_Time;
   double Late_Gate_Close_Time;
   bool Early_Gate_Is_Open;
   bool Late_Gate_Is_Open;
   float Old_Input_Val;
   double Min_Symbol_Period;
   double Max_Symbol_Period;


   AnalogFilterByIir<float> *Filter_Core;
  
};

#endif
