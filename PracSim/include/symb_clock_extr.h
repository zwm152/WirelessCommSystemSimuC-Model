//
//  File = symb_clock_extr.h
//

#ifndef _SYMB_CLOCK_EXTR_H_
#define _SYMB_CLOCK_EXTR_H_

#include "typedefs.h"
#include "signal_T.h"
#include "psmodel.h"


class SymbolClockExtractor : public PracSimModel
{
public:
SymbolClockExtractor( char* instance_nam,
           PracSimModel* outer_model,
           Signal<float> *in_signal,
           Signal<float> *multiplier_output,
           Signal<bit_t> *clock_out );

  ~SymbolClockExtractor(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<float> *In_Sig;
  Signal<float> *Multiplier_Output;
  Signal<bit_t> *Clock_Out;
  double Symb_Width;
  double Samp_Intvl;
  double Integ_Val;
  int Samps_Per_Symb;
  int Samps_Per_Half_Symb;
  float *Temp_Buffer_Start;
  float *Mult_Buffer_Start;
  
};

#endif
