//
//  File = noise_gen.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "noise_gen.h"
#include "model_graph.h"
#include "syst_graph.h"
#include "misdefs.h"
#include "gausrand.h"
extern int PassNumber;
extern ParmFile *ParmInput;
extern SystemGraph CommSystemGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
// normal constructor

GaussianNoiseGenerator::GaussianNoiseGenerator( char* instance_name,
                                              PracSimModel* outer_model,
                                              Signal<float>* noise_sig)
                      :PracSimModel(instance_name,
                                    outer_model)
{
  MODEL_NAME(GaussianNoiseGenerator);

  Noise_Sig = noise_sig;

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Seed);
  GET_DOUBLE_PARM(Noise_Sigma);

  //----------------------------------------------
  MAKE_OUTPUT(Noise_Sig);

}
//=============================================
GaussianNoiseGenerator::~GaussianNoiseGenerator( void ){ };
//===========================================
void GaussianNoiseGenerator::Initialize(void)
{
  *DebugFile << "Now in GaussianNoiseGenerator::Initialize()" << endl;

  Proc_Block_Size = Noise_Sig->GetBlockSize();
}
//=============================================
int GaussianNoiseGenerator::Execute(void)
{
  int is;
  float *noise_sig_ptr;
  float noise_sigma;
  float rand_var;
  float noise_sig_val;

  //--------------------------------------------
  noise_sigma = Noise_Sigma;
  long seed = Seed;

  //---------------------------------------------

  noise_sig_ptr = GET_OUTPUT_PTR(Noise_Sig);

  //-------------------------------------------------
  //  main loop

  for(is=0; is<Proc_Block_Size; is++)
    {
    // generate gaussian RV
    GaussRandom(&seed, &rand_var);

    noise_sig_val = noise_sigma * rand_var;

    *noise_sig_ptr++ = noise_sig_val;
    }// end of main loop

  // put back variables that have changed
  Seed = seed;

  //----------------------------------------------

  return(_MES_AOK);
};

