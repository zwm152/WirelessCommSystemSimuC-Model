//
//  File = downsampler_T.h
//

#ifndef _DOWNSAMPLER_T_H_
#define _DOWNSAMPLER_T_H_

#include "signal_T.h"
#include "control_T.h"
#include "psmodel.h"

template <class T>
class Downsampler : public PracSimModel
{
public:

  Downsampler< T >( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_signal,
                    Signal<T>* out_signal);

  ~Downsampler<T>(void);
  void Initialize(void);
  int Execute(void);

private:
  //---------------------------------------------
  //  user-specified static parameters

   int Decim_Rate;

  //--------------------------------------------
  //  state variables and derived parameters

  int Block_Size;
  double Samp_Intvl;
  int Num_Opening_Drops;

  //T *Start_Of_Buffer;
  //T *End_Of_Buffer;
  //T *Read_Ptr;
  //T *Write_Ptr;

  //--------------------------
  //  Signals

  Signal<T> *In_Sig;
  Signal<T> *Out_Sig;  

};

#endif
