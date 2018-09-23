//
//  File = disc_delay_T.h
//

#ifndef _DISC_DELAY_T_H_
#define _DISC_DELAY_T_H_

#include "signal_T.h"
#include "control_T.h"
#include "delay_modes.h"
#include "psmodel.h"

//======================================================
template <class T>
class DiscreteDelay : public PracSimModel
{
public:
  DiscreteDelay< T >( 
            char* instance_name,
            PracSimModel* outer_model,
            Signal<T>* in_signal,
            Signal<T>* out_signal,
            Control<int>* dynamic_delay_control,
            Control<bool>* delay_change_enable_control);

  DiscreteDelay< T >( 
                  char* instance_name,
                  PracSimModel* outer_model,
                  Signal<T>* in_signal,
                  Signal<T>* out_signal,
                  Control<int>* dynamic_delay_control);

  DiscreteDelay< T >( char* instance_name,
                      PracSimModel* outer_model,
                      Signal<T>* in_signal,
                     Signal<T>* out_signal);

  ~DiscreteDelay<T>(void);
  void Initialize(void);
  int Execute(void);

private:
  void Constructor_Common_Tasks( char* instance_name,
                                 Signal<T>* in_signal,
                                 Signal<T>* out_signal);

  //  user-specified static parameters
  DELAY_MODE_T Delay_Mode;
  int Initial_Delay_In_Samps;
  int Num_Initial_Passes;
  int Max_Delay_In_Samps;
  int Dynamic_Delay_Bias;

  //  state variables and derived parameters
  int Block_Size;
  double Samp_Intvl;
  int Active_Delay_In_Samps;
  int Max_Buffer_Len; 
  T *Start_Of_Buffer;
  T *End_Of_Buffer;
  T *Read_Ptr;
  T *Write_Ptr;

  //  Signals
  Signal<T> *In_Sig;
  Signal<T> *Out_Sig;  

  //  Controls
  Control<bool> *Delay_Chg_Enab_Ctl;
  Control<int> *Dynam_Dly_Ctl;
};

#endif
