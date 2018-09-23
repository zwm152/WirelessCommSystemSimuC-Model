//
//  File = symbgen.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "symb_remap.h"
#include "uni_rand.h"
#include "parmfile.h"
#include "model_graph.h"

extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

SymbRemapper::SymbRemapper( char* instance_name,
                            PracSimModel* outer_model,
                            Signal<byte_t>* in_sig,
                            Signal<byte_t>* out_sig )
              :PracSimModel(  instance_name,
                              outer_model)
{
  MODEL_NAME(SymbRemapper);
  OPEN_PARM_BLOCK;
  GET_INT_PARM(Bits_Per_Symb);

  Out_Sig = out_sig;
  In_Sig = in_sig;
  MAKE_OUTPUT(Out_Sig);
  MAKE_INPUT(In_Sig);

  Num_Symbs = 1;
  int i;
  for(i=1; i<=Bits_Per_Symb; i++)
    Num_Symbs *=2;

  Symb_Map = new byte_t[Num_Symbs];
  int *symb_map = new int[Num_Symbs];
  GET_INT_PARM_ARRAY(symb_map,Num_Symbs);
  for(i=0; i<Num_Symbs; i++)
    {
    Symb_Map[i] = byte_t(symb_map[i]);
    }
}
//===========================================
SymbRemapper::~SymbRemapper( void ){ };

//========================================
void SymbRemapper::Initialize(void)
{
  #ifdef _DEBUG
    *DebugFile << "Now in SymbRemapper::Initialize()" << endl;
  #endif

  //Block_Size = Out_Sig->GetBlockSize();
};
//========================================
int SymbRemapper::Execute()
{
  int is;
  int block_size;
  byte_t *symbs_out, *symbs_in;

  //--------------------------------
  //  Get pointer for output buffer

  symbs_out = GET_OUTPUT_PTR(Out_Sig);
  symbs_in = GET_OUTPUT_PTR(In_Sig);
  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  for (is=0; is<block_size; is++)
    {
    *symbs_out++ = Symb_Map[*symbs_in++];
    }
  return(_MES_AOK);
}

