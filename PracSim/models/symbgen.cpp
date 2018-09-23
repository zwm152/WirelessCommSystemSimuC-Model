//
//  File = symbgen.cpp
//

#include <stdlib.h>
#include <fstream>
#include "symbgen.h"
#include "uni_rand.h"
#include "parmfile.h"
#include "model_graph.h"

extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

SymbGener::SymbGener( char* instance_name,
                      PracSimModel* outer_model,
                      Signal<byte_t>* out_sig )
          :PracSimModel(  instance_name,
                          outer_model)
{
  MODEL_NAME(SymbGener);
  OPEN_PARM_BLOCK;
  GET_LONG_PARM(Initial_Seed);
  Seed = Initial_Seed;
  GET_INT_PARM(Bits_Per_Symb);

  Out_Sig = out_sig;
  MAKE_OUTPUT(Out_Sig);
}
//===========================================
SymbGener::~SymbGener( void ){ };

//========================================
void SymbGener::Initialize(void)
{
  #ifdef _DEBUG
    *DebugFile << "Now in SymbGener::Initialize()" << endl;
  #endif

  Block_Size = Out_Sig->GetBlockSize();
};
//========================================
int SymbGener::Execute()
{
  int is;
  byte_t *symbs_out;
  long seed;

  //--------------------------------
  //  Get pointer for output buffer

  Out_Sig->SetValidBlockSize(Block_Size);
  symbs_out = GET_OUTPUT_PTR(Out_Sig);
  seed = Seed;
  int shift_val = 31 - Bits_Per_Symb;

  for (is=0; is<Block_Size; is++)
    {
    //rand_num = UniformRandom(&seed);
    //test_val = int(Num_Symbs*rand_num);
    //symb_val = test_val;
    //*symbs_out = symb_val;
    *symbs_out = RandomLong(&seed)>>shift_val;
    symbs_out++;
    }
  Seed = seed;
  return(_MES_AOK);
}

