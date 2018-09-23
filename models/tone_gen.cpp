//
//  File = tone_gen.cpp
//

#include <stdlib.h>
#include <fstream>
#include "tone_gen.h"
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

ToneGener::ToneGener( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* out_sig )
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(ToneGener);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Phase_In_Deg);
  GET_DOUBLE_PARM(Freq_In_Hz);
  Phase_In_Cycles = Phase_In_Deg /360.0;

  MAKE_OUTPUT(Out_Sig);
}
//====================================================
ToneGener::~ToneGener( void){};
//====================================================
void ToneGener::Initialize(void)
{
  *DebugFile << "Now in ToneGener::Initialize()" << endl;

  Block_Size = Out_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  Arg_Increm = Freq_In_Hz * Samp_Intvl;
  Base_Arg = Phase_In_Cycles;
}
//====================================================
int ToneGener::Execute()
{
  int is;
  float *out_sig_ptr;
  double base_arg, arg_increm;

  //--------------------------------
  //  Get pointer for output buffer

  Out_Sig->SetValidBlockSize(Block_Size);
  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
  base_arg = Base_Arg;
  arg_increm = Arg_Increm;

  for (is=0; is<Block_Size; is++)
    {
    *out_sig_ptr++ = float(sin(TWO_PI * (base_arg + (is*arg_increm)) ));
    }
  Base_Arg = fmod(Base_Arg + Block_Size*Arg_Increm,1.0);
  return(_MES_AOK);
}

