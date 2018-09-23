//
//  File = mpskoptdem_bp.h
//

#ifndef _MPSKOPTIMDEM_BP_H_
#define _MPSKOPTIMDEM_BP_H_

#include "signal_T.h"
#include "psmodel.h"

class MpskOptimalBandpassDemod : public PracSimModel
{
public:
  MpskOptimalBandpassDemod(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal<float>* in_sig,
                    Signal< bit_t >* symb_clock_in,
                    Signal< byte_t >* out_sig );

  ~MpskOptimalBandpassDemod(void);
  void Initialize(void);
  int Execute(void);

private:
  double Phase_Unbal;
  double Dly_To_Start;
  double Samp_Intvl;
  double Carrier_Freq;
  double Carrier_Freq_Rad;
  double Time;
  int Block_Size;
  Signal< byte_t > *Out_Sig;
  Signal<float>*In_Sig;  
  Signal< bit_t > *Symb_Clock_In;  
  int Bits_Per_Symb;
  int Samps_Per_Symb;
  int Num_Diff_Symbs;
  float *Decis_Bound;
  double *Integ_Val;
  float *Ref_Angle;
};

#endif
