//
//  File = qpsk_err_ctr.h
//

#ifndef _QPSK_ERR_CTR_H_
#define _QPSK_ERR_CTR_H_

#include "signal_T.h"
#include "psmodel.h"
//#include "k_serctr.h"


class QpskErrorCounter : public PracSimModel
{
public:
QpskErrorCounter( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t> *i_in_signal,
            Signal<bit_t> *q_in_signal,
            Signal<bit_t> *i_ref_signal,
            Signal<bit_t> *q_ref_signal );

  ~QpskErrorCounter(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int Symb_Count;
  int Error_Count;
  Signal<bit_t> *I_In_Sig;
  Signal<bit_t> *Q_In_Sig;
  Signal<bit_t> *I_Ref_Sig;
  Signal<bit_t> *Q_Ref_Sig;
  int Num_Holdoff_Passes;
  int Report_Intvl_In_Blocks;
  //k_BerCounter *Kernel;
  //int *Histogram;
  
};

#endif
