//
//  File = ser_ctr.h
//

#ifndef _SER_CTR_H_
#define _SER_CTR_H_

#include "signal_T.h"
#include "psmodel.h"
#include "k_serctr.h"


class SerCounter : public PracSimModel
{
public:
SerCounter( char* instance_name,
            PracSimModel* outer_model,
            Signal<byte_t> *in_signal,
            Signal<byte_t> *ref_signal,
            Signal<byte_t> *error_seq );

  ~SerCounter(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int Symb_Count;
  int Error_Count;
  Signal<byte_t> *In_Sig;
  Signal<byte_t> *Ref_Sig;
  Signal<byte_t> *Error_Seq;
  int Num_Holdoff_Passes;
  int Report_Intvl_In_Blocks;
  k_SerCounter *Kernel;
  //int *Histogram;
  
};

#endif
