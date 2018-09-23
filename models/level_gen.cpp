//
//  File = level_gen.cpp
//

#include <stdlib.h>
#include <fstream>
#include "level_gen.h"
//#include "uni_rand.h"
#include "misdefs.h"
#include "parmfile.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"

extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

LevelGener::LevelGener( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* out_sig )
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(LevelGener);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

  //GET_LONG_PARM(Initial_Seed);
  //Seed = Initial_Seed;
  //GET_LONG_PARM(Block_Size);

  GET_FLOAT_PARM(Const_Level);

  MAKE_OUTPUT(Out_Sig);
}
//====================================================
LevelGener::~LevelGener( void){};
//====================================================
void LevelGener::Initialize(void)
{
  *DebugFile << "Now in LevelGener::Initialize()" << endl;

  Block_Size = Out_Sig->GetBlockSize();
}
//====================================================
int LevelGener::Execute()
{
  int is;
  float *out_sig_ptr;

  //--------------------------------
  //  Get pointer for output buffer

  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);

  for (is=0; is<Block_Size; is++)
    {
    *out_sig_ptr++ = Const_Level;
    }
  Out_Sig->SetValidBlockSize(Block_Size);
  return(_MES_AOK);
}

