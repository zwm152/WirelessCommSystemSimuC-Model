//
//  File = bscerrorgen.h
//

#ifndef _BSCERRORGEN_H_
#define _BSCERRORGEN_H_

#include "signal_T.h"
#include "psmodel.h"


class BscErrorGener : public PracSimModel
{
public:
   BscErrorGener( char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<bit_t> *in_signal,
                  Signal<bit_t> *out_signal );

   ~BscErrorGener(void);
   void Initialize(void);
   int Execute(void);

private:
   int Block_Size;
   double Bit_Error_Probability;
   long Initial_Seed;
   long Working_Seed;
   long Decis_Thresh;

   Signal<bit_t> *In_Sig;
   Signal<bit_t> *Out_Sig;  
};

#endif
