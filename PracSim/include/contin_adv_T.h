//
//  File = contin_adv_T.h
//

#ifndef _CONTIN_ADV_T_H_
#define _CONTIN_ADV_T_H_

#include "signal_T.h"
#include "control_T.h"
#include "adv_modes.h"
#include "interp_modes.h"
#include "psmodel.h"

template <class T>
class ContinuousAdvance : public PracSimModel
{
public:

  //---------------------------------------------
  // constructor that supports all advance modes

  ContinuousAdvance< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal,
                    Control<float>* new_adv,
                    Control<bool>* advance_change_enabled);

  //---------------------------------------------------
  //  constructor does not support 'gated' advance mode
  //  (eliminates the need to connect gating control)

  ContinuousAdvance< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal,
                    Control<float>* new_adv);

  //---------------------------------------------------
  //  constructor does not support 'gated' or 'dynamic'
  //  advance modes (eliminates the need to connect gating
  //  and delay controls)

  ContinuousAdvance< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal);

  ~ContinuousAdvance<T>(void);
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

  ADVANCE_MODE_T Advance_Mode;
  INTERP_MODE_T Interp_Mode;
  double Initial_Advance;
  int Max_Advance;

  //--------------------------------------------
  //  state variables and derived parameters

  double Block_Size;
  double Samp_Intvl;

  double Active_Advance;
  float Interp_Weight;
  float One_Minus_Weight;
  int Active_Buffer_Len;
  int Num_Sidelobes;

  int Max_Buffer_Len;  // numb of locs needed for Max_Delay

  T *Start_Of_Buffer;
  T *End_Of_Buffer;
  T *Read_Ptr;
  T *Write_Ptr;

  //--------------------------
  //  Signals

  Signal<T> *In_Sig;
  Signal<T> *Out_Sig;  

  //-----------------------------------
  //  Controls

  Control<bool> *Advance_Change_Enabled;
  Control<float> *New_Advance;
};

#endif
