//
//  File = mpskidealmod.h
//

#ifndef _MPSKIDEALMOD_H_
#define _MPSKIDEALMOD_H_

#include "signal_T.h"
#include "psmodel.h"

class MpskIdealModulator : public PracSimModel
{
public:
  MpskIdealModulator(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<byte_t>* in_sig,
                  Signal< std::complex<float> >* cmpx_out_sig,
                  Signal<bit_t>* symb_clock_out,
                  Signal<float>* mag_out_sig,
                  Signal<float>* phase_out_sig );

  ~MpskIdealModulator(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Amp_Unbal;
  double Real_Unbal;
  double Imag_Unbal;
  int In_Block_Size;
  int Out_Block_Size;
  int Bits_Per_Symb;
  int Num_Diff_Symbs;
  int Samps_Per_Symb;
  double Symb_Duration;
  float *I_Compon;
  float *Q_Compon;
  Signal<byte_t> *In_Sig;
  Signal<bit_t> *Symb_Clock_Out;
  Signal< std::complex<float> > *Cmpx_Out_Sig;  
  Signal<float> *Mag_Out_Sig;
  Signal<float> *Phase_Out_Sig;
};

#endif
