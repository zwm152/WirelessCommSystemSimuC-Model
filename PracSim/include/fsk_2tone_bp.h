//
//  File = vco_bp.h
//

#ifndef _FSK_2TONE_BP_H_
#define _FSK_2TONE_BP_H_

#include "signal_T.h"
#include "psmodel.h"

class FskTwoToneModulator_Bp : public PracSimModel
{
public:
  FskTwoToneModulator_Bp(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal<float>* out_sig );

  ~FskTwoToneModulator_Bp(void);
  void Initialize(void);
  int Execute(void);

private:
  double Freq_Hi_Hz;
  double Freq_Lo_Hz;
  float Lo_Control_Val;
  float Hi_Control_Val;
  double Hz_Per_Volt;
  int Block_Size;
  double Samp_Intvl;
  double Cumul_Phase_Hi;
  double Cumul_Phase_Lo;
  double Slope_Hi_On;
  double Slope_Lo_On;
  double Full_Scale_Output_Level;
  double Time;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;  
};

#endif
