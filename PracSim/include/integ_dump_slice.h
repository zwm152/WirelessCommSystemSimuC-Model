//
//  File = integ_dump_slice.h
//

#ifndef _INTEG_DUMP_SLICE_H_
#define _INTEG_DUMP_SLICE_H_

#include "typedefs.h"
#include "signal_T.h"
#include "psmodel.h"


class IntegrateDumpAndSlice : public PracSimModel
{
public:
IntegrateDumpAndSlice( char* instance_nam,
           PracSimModel* outer_model,
           Signal<float> *in_signal,
           //Signal<float> *corr_sig, //use CorrelateDumpAndSlice if this sig is needed
           Signal<bit_t> *symb_clock_in,
           Signal<float> *samp_wave_out,
           Signal<bit_t> *out_signal );

  ~IntegrateDumpAndSlice(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Block_Size;
  int In_Block_Size;
  Signal<float> *In_Sig;
  //Signal<float> *Corr_Sig;
  Signal<float> *Samp_Wave_Out;
  Signal<bit_t> *Out_Sig;
  Signal<bit_t> *Symb_Clock_In;
  double Symb_Width;
  double In_Samp_Intvl;
  double Out_Samp_Intvl;
  double Integ_Val;
  
};

#endif
