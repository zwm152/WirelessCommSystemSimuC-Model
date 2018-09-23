//
//  File = ramp_gen.cpp
//

#include <stdlib.h>
#include <fstream>
#include "ramp_gen.h"
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

RampGener::RampGener( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* out_sig )
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(RampGener);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Rise_Per_Sample);

  MAKE_OUTPUT(Out_Sig);
}
//====================================================
RampGener::~RampGener( void){};
//====================================================
void RampGener::Initialize(void)
{
  *DebugFile << "Now in RampGener::Initialize()" << endl;

  Block_Size = Out_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  Samp_Count = 0;
}
//====================================================
int RampGener::Execute()
{
  int is;
  float *out_sig_ptr;

  //--------------------------------
  //  Get pointer for output buffer

  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);

  for (is=0; is<Block_Size; is++)
    {
    *out_sig_ptr++ = float(Samp_Count * Rise_Per_Sample);
    Samp_Count++;
    }
  return(_MES_AOK);
}

