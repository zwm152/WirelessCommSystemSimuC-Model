//
//  File = contin_delay_tester.h
//

#ifndef _CONTIN_DELAY_TESTER_H_
#define _CONTIN_DELAY_TESTER_H_

#include "signal_T.h"
#include "control_T.h"
#include "psmodel.h"

template <class T>
class ContinuousDelayTester : public PracSimModel
{
public:
  ContinuousDelayTester< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal,
                    Control<float>* delay_value,
                    Control<bool>* delay_change_enabled);

  ~ContinuousDelayTester<T>(void);
  void Initialize(void);
  int Execute(void);

private:

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;
  double Samp_Intvl;

  //--------------------------
  //  Signals

  Signal<T> *In_Sig;
  Signal<T> *Out_Sig;  

  //-----------------------------------
  //  Controls

  Control<float> *Delay_Value;
  Control<bool> *Delay_Change_Enabled;
};

#endif
