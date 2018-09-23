//
//  File = ber_ctr.h
//

#ifndef _BER_CTR_H_
#define _BER_CTR_H_

#include "signal_T.h"
#include "psmodel.h"
#include "k_berctr.h"


class BerCounter : public PracSimModel
{
public:
BerCounter( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t> *in_signal,
            Signal<bit_t> *ref_signal );

  ~BerCounter(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int Symb_Count;
  int Error_Count;
  Signal<bit_t> *In_Sig;
  Signal<bit_t> *Ref_Sig;
  int Num_Holdoff_Passes;
  int Report_Intvl_In_Blocks;
  k_BerCounter *Kernel;
  //int *Histogram;
  
};

#endif
