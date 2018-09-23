//
//  File = dice.h
//

#ifndef _DICE_H_
#define _DICE_H_

#include "psmodel.h"
//#include "typedefs.h"

#include "signal_T.h"
class DiceRoller : public PracSimModel
{
public:
  DiceRoller( char* instance_nam,
             PracSimModel* outer_model,
             Signal<byte_t>* out_sig);

  ~DiceRoller(void);
  void Initialize(void);
  int Execute(void);

private:
  long Initial_Seed;
  long Seed;
  int Block_Size;
  int Faces_Per_Die;
  int Num_Symbs;
  byte_t Old_Face_Val;
  Signal<byte_t> *Out_Sig;
  
};

#endif //_SYMBGEN_H_
