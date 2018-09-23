//
//  File = dir1_fir.h
//

#ifndef _DIR1_FIR_H_
#define _DIR1_FIR_H_

#include "psmodel.h"
#include "intsig.h"
#include "bytesig.h"
#include "fir_dsgn.h"

class DirectFormFir : public PracSimModel
{
public:
  DirectFormFir(  char* instance_name,
                  PracSimModel* outer_model,
                  intSignal* in_sig,
                  intSignal* out_sig );

  ~DirectFormFir(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  double Samp_Rate;
  intSignal *In_Sig;
  intSignal *Out_Sig;
  int Kind_Of_Filter_Resp;
  FirFilterDesign *Filter_Design;
  int Num_Taps;
  int *Coeff;
  int *Input_Mem;
  int Write_Idx;
  long Output_Quan_Factor;
};

#endif
