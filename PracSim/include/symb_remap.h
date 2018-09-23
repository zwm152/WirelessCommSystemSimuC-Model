//
//  File = symb_remap.h
//

#ifndef _SYMB_REMAP_H_
#define _SYMB_REMAP_H_

#include "psmodel.h"
//#include "typedefs.h"

#include "signal_T.h"
class SymbRemapper : public PracSimModel
{
public:
  SymbRemapper(  char* instance_nam,
                 PracSimModel* outer_model,
                 Signal<byte_t>* in_sig,
                 Signal<byte_t>* out_sig);

  ~SymbRemapper(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int Bits_Per_Symb;
  int Num_Symbs;
  byte_t *Symb_Map;
  Signal<byte_t> *In_Sig;
  Signal<byte_t> *Out_Sig;
  
};

#endif //_SYMBGEN_H_
