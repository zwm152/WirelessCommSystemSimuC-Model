//
//  File = dice_analyzer.h
//

#ifndef _DICE_ANALYZER_H_
#define _DICE_ANALYZER_H_

#include "signal_T.h"
#include "psmodel.h"
//#include "k_berctr.h"


class DiceAnalyzer : public PracSimModel
{
public:
DiceAnalyzer( char* instance_name,
            PracSimModel* outer_model,
            Signal<bit_t> *in_signal );

  ~DiceAnalyzer(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int *Symb_Count;
  int *Double_Count;
  Signal<byte_t> *In_Sig;
  int Report_Intvl_In_Blocks;
  int Faces_Per_Die;
  //int Num_Diff_Symbs;
  //int *Histogram;
  
};

#endif
