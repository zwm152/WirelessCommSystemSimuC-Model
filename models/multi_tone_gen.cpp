//
//  File = multi_tone_gen.cpp
//
#include <stdlib.h>
#include <fstream>
#include "multi_tone_gen.h"
#include "misdefs.h"
#include "parmfile.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"
#include <iostream>
using namespace std;

extern int PassNumber;
extern ParmFile *ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

MultipleToneGener::MultipleToneGener( 
                     char* instance_name,
                     PracSimModel* outer_model,
                     Signal<float>* out_sig )

                  :PracSimModel( instance_name,
                                 outer_model )
{
  MODEL_NAME(MultipleToneGener);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

  GET_INT_PARM(Num_Sines);
  GET_DOUBLE_PARM(Holdoff_Delay);

  Phase_In_Deg = new double[Num_Sines];
  Phase_In_Rad = new double[Num_Sines];
  Arg_Increm = new double[Num_Sines];
  Base_Arg = new double[Num_Sines];
  Freq_In_Hz = new double[Num_Sines];
  Tone_Gain = new double[Num_Sines];

  GET_DOUBLE_PARM_ARRAY(Phase_In_Deg,Num_Sines);

  for(int idx=0; idx<Num_Sines; idx++) {
    Phase_In_Rad[idx] = PI * Phase_In_Deg[idx] /180.0;
  }
  GET_DOUBLE_PARM_ARRAY(Freq_In_Hz, Num_Sines);
  GET_DOUBLE_PARM_ARRAY(Tone_Gain, Num_Sines);
  MAKE_OUTPUT(Out_Sig);
}
//====================================================
MultipleToneGener::~MultipleToneGener( void)
{
  delete[] Phase_In_Deg;
  delete[] Phase_In_Rad;
  delete[] Arg_Increm;
  delete[] Base_Arg;
  delete[] Freq_In_Hz;
  delete[] Tone_Gain;
};
//====================================================
void MultipleToneGener::Initialize(void)
{
#ifdef _DEBUG
  *DebugFile 
      << "Now in MultipleToneGener::Initialize()" 
      << endl;
#endif

  Block_Size = Out_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();

  for(int isin=0; isin<Num_Sines; isin++) {
    Arg_Increm[isin] = Freq_In_Hz[isin] * Samp_Intvl;
    Base_Arg[isin] = (Phase_In_Deg[isin]/360.0) - 
                     (Holdoff_Delay*Freq_In_Hz[isin]);
  }
}
//====================================================
int MultipleToneGener::Execute()
{
  int is, isin;
  float *out_sig_ptr;
  double out_samp;
  double *base_arg, *arg_increm, *tone_gain;

  //--------------------------------
  //  Get pointer for output buffer
  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
  base_arg = Base_Arg;
  arg_increm = Arg_Increm;
  tone_gain = Tone_Gain;
  Out_Sig->SetValidBlockSize(Block_Size);

  for (is=0; is<Block_Size; is++){
    out_samp = 0;
    for(isin=0; isin<Num_Sines; isin++){
      out_samp += tone_gain[isin] * sin(TWO_PI * 
         (base_arg[isin] + (is*arg_increm[isin])) );
    }
    if(is < Block_Size/2){
      *out_sig_ptr++ = float(out_samp);
    }
    else {
      *out_sig_ptr++ = float(out_samp);
    }
  }
  for(isin=0; isin<Num_Sines; isin++){
    Base_Arg[isin] = fmod(Base_Arg[isin] + 
                     Block_Size*Arg_Increm[isin],1.0);
  }
  return(_MES_AOK);
}