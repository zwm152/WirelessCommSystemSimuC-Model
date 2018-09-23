//
//  File = sines_in_awgn.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "sines_in_awgn.h"
#include "misdefs.h"
#include "parmfile.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"
#include "gausrand.h"

extern ParmFile *ParmInput;
//extern ActiveSystemGraph ActSystGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

SinesInAwgn::SinesInAwgn( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* out_sig )
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(SinesInAwgn);
  Out_Sig = out_sig;
  OPEN_PARM_BLOCK;

   GET_DOUBLE_PARM(Awgn_Sigma);
   GET_LONG_PARM(Noise_Seed);
  GET_INT_PARM(Num_Sines);

  Phase_In_Deg = new double[Num_Sines];
  Phase_In_Rad = new double[Num_Sines];
  Arg_Increm = new double[Num_Sines];
  Base_Arg = new double[Num_Sines];
  Freq_In_Hz = new double[Num_Sines];
  Tone_Gain = new double[Num_Sines];

  GET_DOUBLE_PARM_ARRAY(Phase_In_Deg,Num_Sines);
  for(int idx=0; idx<Num_Sines; idx++)
    {
    Phase_In_Rad[idx] = PI * Phase_In_Deg[idx] /180.0;
    cout << "Phase_In_Rad[" << idx << "] = " << Phase_In_Rad[idx] << endl;
    }
  GET_DOUBLE_PARM_ARRAY(Freq_In_Hz, Num_Sines);
  GET_DOUBLE_PARM_ARRAY(Tone_Gain, Num_Sines);
  MAKE_OUTPUT(Out_Sig);
}
//====================================================
SinesInAwgn::~SinesInAwgn( void){};
//====================================================
void SinesInAwgn::Initialize(void)
{
  *DebugFile << "Now in SinesInAwgn::Initialize()" << endl;

  Block_Size = Out_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();

  for(int isin=0; isin<Num_Sines; isin++)
    {
    Arg_Increm[isin] = TWO_PI * Freq_In_Hz[isin] * Samp_Intvl;
    Base_Arg[isin] = Phase_In_Rad[isin];
    }
}
//====================================================
int SinesInAwgn::Execute()
{
  int is, isin;
  float *out_sig_ptr;
  double out_samp;
  double *base_arg, *arg_increm;
  double *tone_gain;
  long noise_seed;
  double awgn_sigma;

  //--------------------------------
  //  Get pointer for output buffer

  out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
  base_arg = Base_Arg;
  arg_increm = Arg_Increm;
  tone_gain = Tone_Gain;
  noise_seed = Noise_Seed;
  awgn_sigma = Awgn_Sigma;

  for (is=0; is<Block_Size; is++)
    {
    GaussRandom(&noise_seed, &out_samp);
    out_samp *= awgn_sigma;
    for(isin=0; isin<Num_Sines; isin++)
      {
      out_samp += tone_gain[isin] * sin(base_arg[isin] + (is*arg_increm[isin]));
      }
    if(is < Block_Size/2)
      {
      //*out_sig_ptr++ = 2*is*out_samp/Block_Size;
      *out_sig_ptr++ = out_samp;
      }
    else
      {
      //*out_sig_ptr++ = out_samp * (1.0- 2*(is-Block_Size/2.0)/double(Block_Size));
      *out_sig_ptr++ = out_samp;
      }
    }
  for(isin=0; isin<Num_Sines; isin++)
    {
    Base_Arg[isin] = fmod(Base_Arg[isin] + Block_Size*Arg_Increm[isin],TWO_PI);
    }
  Noise_Seed = noise_seed;
  return(_MES_AOK);
}

