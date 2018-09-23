//
//  File = discrete_adv_T.h
//

#ifndef _DISCRETE_ADV_T_H_
#define _DISCRETE_ADV_T_H_

#include "signal_T.h"
#include "psmodel.h"
#include "adv_modes.h"
#include "control_T.h"


template < class T >
class DiscreteAdvance : public PracSimModel
{
public:
  DiscreteAdvance< T >( char* instance_nam,
                   PracSimModel* outer_model,
                   Signal<T> *in_signal,
                   Signal<T> *out_signal,
                   Control<int> *dynam_adv_cntrl,
                   Control<bool> *adv_chg_enab_cntrl );
  DiscreteAdvance< T >( char* instance_nam,
                   PracSimModel* outer_model,
                   Signal<T> *in_signal,
                   Signal<T> *out_signal,
                   Control<int> *dynam_adv_cntrl );
  DiscreteAdvance< T >( char* instance_nam,
                   PracSimModel* outer_model,
                   Signal<T> *in_signal,
                   Signal<T> *out_signal );

  ~DiscreteAdvance< T >(void);
  void Initialize(void);
  int Execute(void);

private:
  void Constructor_Common_Tasks( char *instance_name,
                            Signal<T> *in_sig,
                            Signal<T> *out_sig);
  Signal<T> *In_Sig;
  Signal<T> *Out_Sig;
  Control<int> *Dynam_Adv_Cntrl;
  Control<bool> *Adv_Chg_Enab_Cntrl;

  //---------------------------------------------
  //  user-specified static parameters

  ADVANCE_MODE_T Advance_Mode;
  int Initial_Adv_In_Samps;
  int Max_Adv_In_Samps;
  int Num_Initial_Passes;

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;

  int Max_Buffer_Len;

  T *Start_Of_Buffer;
  T *End_Of_Buffer;
  T *Read_Ptr;
  T *Write_Ptr;

  int Active_Adv_In_Samps;
  int Whole_Blocks_To_Adv;
  int Blocks_Of_Offset;
  int Num_Blocks_Skipped;
  int Return_Status;
  int New_Pass_Number;
};

#endif
