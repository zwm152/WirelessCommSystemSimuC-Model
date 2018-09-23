//
//  File = andf1fir.h
//

#ifndef _ANDF1FIR_H_
#define _ANDF1FIR_H_

#include "psmodel.h"
#include "signal_T.h"
#include "fir_dsgn.h"

class AnlgDirectFormFir : public PracSimModel
{
public:
  AnlgDirectFormFir(  char* instance_name,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal<float>* out_sig );

  ~AnlgDirectFormFir(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  double Samp_Intvl;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;
  int Kind_Of_Filter_Resp;
  FirFilterDesign *Filter_Design;
  int Num_Taps;
  float *Coeff;
  float *Input_Mem;
  double Fixed_Gain;
  char* Coeff_Fname;
  ifstream *Coeff_File;
  bool Bypass_Enabled;
};

#endif
