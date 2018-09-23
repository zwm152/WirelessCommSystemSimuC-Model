//
//  File = msk_genie.h
//

#ifndef _MSK_GENIE_H_
#define _MSK_GENIE_H_

#include "signal_T.h"

class MskShapeGenie : public PracSimModel
{
public:
  MskShapeGenie( char* instance_name,
                PracSimModel *outer_model,
                Signal< float >* shape_signal_i,
                Signal< float >* shape_signal_q );

  ~MskShapeGenie(void);
  void Initialize(void);
  int Execute(void);

private:
  double Pi_Over_Bit_Dur;
  double Bit_Durat;
  double Samp_Intvl;
  int Shaping_Is_Bipolar;
  int Samps_Out_Cnt;
  int Block_Size;
  Signal< float > *I_Shape_Sig;
  Signal< float > *Q_Shape_Sig;
};

#endif
