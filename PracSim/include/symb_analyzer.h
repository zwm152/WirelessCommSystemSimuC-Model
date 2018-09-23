//
//  File = bit_analyzer.h
//

#ifndef _SYMB_ANALYZER_H_
#define _SYMB_ANALYZER_H_

#include "signal_T.h"
#include "psmodel.h"
//#include "k_berctr.h"


class SymbSeqAnalyzer : public PracSimModel
{
public:
SymbSeqAnalyzer( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t> *in_signal );

  ~SymbSeqAnalyzer(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int *Symb_Count;
  int *Double_Count;
  Signal<byte_t> *In_Sig;
  int Report_Intvl_In_Blocks;
  int Bits_Per_Symb;
  int Num_Diff_Symbs;
  //int *Histogram;
  
};

#endif
