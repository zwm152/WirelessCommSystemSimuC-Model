//
//  File = symbgen.h
//

#ifndef _SYMBGEN_H_
#define _SYMBGEN_H_

#include "psmodel.h"
//#include "typedefs.h"

#include "signal_T.h"
class SymbGener : public PracSimModel
{
public:
  SymbGener( char* instance_nam,
             PracSimModel* outer_model,
             Signal<byte_t>* out_sig);

  ~SymbGener(void);
  void Initialize(void);
  int Execute(void);

private:
  long Initial_Seed;
  long Seed;
  int Block_Size;
  int Bits_Per_Symb;
  int Num_Symbs;
  Signal<byte_t> *Out_Sig;
  
};

#endif //_SYMBGEN_H_
