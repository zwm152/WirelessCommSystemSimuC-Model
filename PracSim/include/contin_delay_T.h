//
//  File = contin_delay_T.h
//

#ifndef _CONTIN_DELAY_T_H_
#define _CONTIN_DELAY_T_H_

#include "signal_T.h"
#include "control_T.h"
#include "delay_modes.h"
#include "interp_modes.h"
#include "psmodel.h"

template <class T>
class ContinuousDelay : public PracSimModel
{
public:

  //---------------------------------------------
  // constructor that supports all delay modes

  ContinuousDelay< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal,
                    Control<float>* new_delay,
                    Control<bool>* delay_change_enabled);

  //---------------------------------------------------
  //  constructor does not support 'gated' delay mode
  //  (eliminates the need to connect gating control)

  ContinuousDelay< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal,
                    Control<float>* new_delay);

  //---------------------------------------------------
  //  constructor does not support 'gated' or 'dynamic'
  //  delay modes (eliminates the need to connect gating
  //  and delay controls)

  ContinuousDelay< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal);

  ~ContinuousDelay<T>(void);
  void Initialize(void);
  int Execute(void);

private:
  //------------------------------------------------------
  //  private method that is called by each of the
  //  three different constructors to perform common tasks

  void Constructor_Common_Tasks(  char* instance_name,
                                  Signal<T>* in_signal,
                                  Signal<T>* out_signal);

  //---------------------------------------------
  //  user-specified static parameters

  DELAY_MODE_T Delay_Mode;
  INTERP_MODE_T Interp_Mode;
  double Initial_Delay;
  int Max_Delay;

  //--------------------------------------------
  //  state variables and derived parameters

  double Block_Size;
  double Samp_Intvl;

  double Active_Delay;
  float Interp_Weight;
  float One_Minus_Weight;
  int Active_Buffer_Len;
  int Num_Sidelobes;
  int Return_Status;
  int New_Pass_Number;
  int Blocks_Of_Offset;
  int Num_Blocks_Skipped;
  int Offset_Sum_Start_To_Out_Samp;
  float *Sinc_Val;

  int Max_Buffer_Len;  // numb of locs needed for Max_Delay

  T *Start_Of_Buffer;
  T *End_Of_Buffer;
  T *Read_Ptr;
  T *Read_Ptr_Start;
  T *Write_Ptr;

  //--------------------------
  //  Signals

  Signal<T> *In_Sig;
  Signal<T> *Out_Sig;  

  //-----------------------------------
  //  Controls

  Control<bool> *Delay_Change_Enabled;
  Control<float> *New_Delay;
};

#endif
