//
//  File = integ_n_dump.h
//

#ifndef _INTEG_N_DUMP_H_
#define _INTEG_N_DUMP_H_

#include "typedefs.h"
#include "signal_T.h"
#include "psmodel.h"


class IntegrateAndDump : public PracSimModel
{
public:
IntegrateAndDump( char* instance_nam,
           PracSimModel* outer_model,
           Signal<float> *in_signal,
           Signal<bit_t> *symb_clock_in,
           Signal<float> *samp_wave_out );

  ~IntegrateAndDump(void);
  void Initialize(void);
  int Execute(void);

private:
  int Out_Block_Size;
  int In_Block_Size;
  Signal<float> *In_Sig;
  Signal<float> *Samp_Wave_Out;
  Signal<bit_t> *Symb_Clock_In;
  double Symb_Width;
  double In_Samp_Intvl;
  double Out_Samp_Intvl;
  double Integ_Val;
  double Integ_Gain;
  
};

#endif
