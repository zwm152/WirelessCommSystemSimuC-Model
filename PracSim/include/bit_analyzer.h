//
//  File = bit_analyzer.h
//

#ifndef _BIT_ANALYZER_H_
#define _BIT_ANALYZER_H_

#include "signal_T.h"
#include "psmodel.h"
//#include "k_berctr.h"


class BitSeqAnalyzer : public PracSimModel
{
public:
BitSeqAnalyzer( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t> *in_signal );

  ~BitSeqAnalyzer(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int Symb_Count;
  Signal<bit_t> *In_Sig;
  int Report_Intvl_In_Blocks;
  int Zeros_Count;
  int Ones_Count;
  //int *Histogram;
  
};

#endif
