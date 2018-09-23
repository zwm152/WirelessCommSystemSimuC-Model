//
//  File = dice.cpp
//

#include <stdlib.h>
#include <fstream>
#include "dice.h"
#include "uni_rand.h"
#include "parmfile.h"
#include "model_graph.h"

extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
#define MODULUS 2147483647
#define SIXTH_MODULUS 357913941.1667

//======================================================

DiceRoller::DiceRoller( char* instance_name,
                      PracSimModel* outer_model,
                      Signal<byte_t>* out_sig )
          :PracSimModel(  instance_name,
                          outer_model)
{
  MODEL_NAME(DiceRoller);
  OPEN_PARM_BLOCK;
  GET_LONG_PARM(Initial_Seed);
  Seed = Initial_Seed;
  GET_INT_PARM(Faces_Per_Die);

  Out_Sig = out_sig;
  MAKE_OUTPUT(Out_Sig);
}
//===========================================
DiceRoller::~DiceRoller( void ){ };

//========================================
void DiceRoller::Initialize(void)
{
  #ifdef _DEBUG
    *DebugFile << "Now in DiceRoller::Initialize()" << endl;
  #endif

  Block_Size = Out_Sig->GetBlockSize();
};
//========================================
int DiceRoller::Execute()
{
  int is;
  byte_t *symbs_out;
  byte_t face_val, old_face_val;
  long seed;

  //--------------------------------
  //  Get pointer for output buffer

  symbs_out = GET_OUTPUT_PTR(Out_Sig);
  seed = Seed;
  old_face_val = Old_Face_Val;

  for (is=0; is<Block_Size; is++)
    {
    //*symbs_out = RandomLong(&seed)>>shift_val;
    //*symbs_out = 1+ (RandomLong(&seed)%Faces_Per_Die);
    face_val = old_face_val;
    //while((face_val==old_face_val) || ((face_val+old_face_val) == 7))
    //{
      face_val = 1+ byte_t(RandomLong(&seed)/SIXTH_MODULUS);
    //}
    old_face_val = face_val;
    *symbs_out = face_val;
    symbs_out++;
    }
   Old_Face_Val = old_face_val;
  Seed = seed;
  Out_Sig->SetValidBlockSize(Block_Size);
  return(_MES_AOK);
}

