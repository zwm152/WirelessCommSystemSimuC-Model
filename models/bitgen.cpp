//
//  File = bitgen.cpp
//

#include <stdlib.h>
#include <fstream>
#include "bitgen.h"
#include "uni_rand.h"
#include "parmfile.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"

extern ParmFile *ParmInput;
extern SystemGraph CommSystemGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

BitGener::BitGener( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<bit_t>* out_sig )
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(BitGener);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

  GET_LONG_PARM(Initial_Seed);
  Seed = Initial_Seed;
  //GET_LONG_PARM(Block_Size);

  MAKE_OUTPUT(Out_Sig);
}
//====================================================
BitGener::~BitGener( void){};
//====================================================
void BitGener::Initialize(void)
{
  #ifdef _DEBUG
    *DebugFile << "Now in BitGener::Initialize()" << endl;
  #endif

  Block_Size = Out_Sig->GetBlockSize();
}
//====================================================
int BitGener::Execute()
{
  int is;
  bit_t bit_val;
  long seed;
  //float rand_num;
  bit_t *bits_out;

  //--------------------------------
  //  Get pointer for output buffer

  Out_Sig->SetValidBlockSize(Block_Size);
  bits_out = GET_OUTPUT_PTR(Out_Sig);
  seed = Seed;

  for (is=0; is<Block_Size; is++)
    {
    bit_val = RandomBit(&seed);
//    rand_num = UniformRandom(&seed);
//    if(rand_num >= 0.5)
//      {
//      bit_val = 1;
//      }
//    else
//      {bit_val = 0; //usual setting
      //bit_val = -1; //temp change for RRC eval
//      }
    *bits_out = bit_val;
    //*bits_out = 1;
    bits_out++;
    }
  Seed = seed;
  return(_MES_AOK);
}

