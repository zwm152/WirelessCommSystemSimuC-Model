//
//  File = k_berctr.h
//

#ifndef _K_BERCTR_H_
#define _K_BERCTR_H_

#include "typedefs.h"
#include "signal_T.h"
#include "psmodel.h"


class k_BerCounter
{
public:
  k_BerCounter( char* instance_name );
  k_BerCounter( char* instance_name,
                int num_holdoff_passes,
                int report_intvl_in_blocks);

  ~k_BerCounter(void);
  void Initialize(void);
  int Execute(  bit_t * in_sig,
                bit_t * ref_sig,
                int block_size);

private:
  int Block_Size;
  int Bit_Count;
  int Bit_0_Count;
  int Bit_1_Count;
  int Error_Count;
  int Error_0_Count;
  int Error_1_Count;
  Signal<bit_t> *In_Sig;
  Signal<bit_t> *Ref_Sig;
  int Num_Holdoff_Passes;
  int Report_Intvl_In_Blocks;
  char *Instance_Name;
  
};

#endif
