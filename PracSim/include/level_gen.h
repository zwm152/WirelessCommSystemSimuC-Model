//
//  File = level_gen.h
//

#ifndef _LEVEL_GEN_H_
#define _LEVEL_GEN_H_

#include "psmodel.h"
#include "signal_T.h"

class LevelGener : public PracSimModel
{
public:
  LevelGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<float>* out_sig );

  ~LevelGener(void);
  void Initialize(void);
  int Execute(void);

private:
  //long Initial_Seed;
  //long Seed;
  int Block_Size;
  double Samp_Intvl;

  float Const_Level;

  Signal<float> *Out_Sig;
  
};

#endif //_LEVEL_GEN_H_
