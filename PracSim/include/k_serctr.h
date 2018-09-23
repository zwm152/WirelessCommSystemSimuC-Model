//
//  File = k_serctr.h
//

#ifndef _K_SERCTR_H_
#define _K_SERCTR_H_

#include "typedefs.h"
#include "signal_T.h"
#include "psmodel.h"


class k_SerCounter
{
public:
  k_SerCounter( char* instance_name );
  k_SerCounter( char* instance_name,
                int num_holdoff_passes,
                int report_intvl_in_blocks);

  ~k_SerCounter(void);
  void Initialize(void);
  int Execute(  byte_t * in_sig,
                byte_t * ref_sig,
                byte_t * error_seq,
                int block_size);

private:
  int Block_Size;
  int Symb_Count;
  int Error_Count;
  Signal<byte_t> *In_Sig;
  Signal<byte_t> *Ref_Sig;
  Signal<byte_t> *Error_Seq;
  int Num_Holdoff_Passes;
  int Report_Intvl_In_Blocks;
  char *Instance_Name;
  
};

#endif
