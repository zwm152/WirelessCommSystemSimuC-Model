//
//  File = rate_changer_T.h
//

#ifndef _RATE_CHANGER_T_H_
#define _RATE_CHANGER_T_H_

#include "signal_T.h"
#include "control_T.h"
#include "delay_modes.h"
#include "interp_modes.h"
#include "psmodel.h"

template <class T>
class RateChanger : public PracSimModel
{
public:

  //---------------------------------------------------
  //  constructor does not support 'gated' or 'dynamic'
  //  delay modes (eliminates the need to connect gating
  //  and delay controls)

  RateChanger< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal);

  ~RateChanger<T>(void);
  void Initialize(void);
  int Execute(void);

private:

  //--------------------------------------------
  //  state variables and derived parameters

  double Nom_Block_Size;
  double Samp_Intvl;
  double Out_Samp_Intvl;

  double Rate_Change_Factor;

  int Num_Sidelobes;
  float *Sinc_Val;
  int Out_Samp_Count;
  int Rt_Brack_Avail;

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

};

#endif
