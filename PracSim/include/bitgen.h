//
//  File = bitgen.h
//

#ifndef _BITGEN_H_
#define _BITGEN_H_

#include "psmodel.h"
#include "signal_T.h"

class BitGener : public PracSimModel
{
public:
  BitGener( char* instance_nam,
            PracSimModel *outer_model,
            Signal<bit_t>* out_sig );

  ~BitGener(void);
  void Initialize(void);
  int Execute(void);

private:
  long Initial_Seed;
  long Seed;
  int Block_Size;
  //double Samp_Rate;
  Signal<bit_t> *Out_Sig;
  
};

#endif //_BITGEN_H_
