//
//  File = noise_gen.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "rayl_gen.h"
#include "model_graph.h"
#include "syst_graph.h"
#include "misdefs.h"
#include "raylrand.h"
extern int PassNumber;
extern ParmFile *ParmInput;
extern SystemGraph CommSystemGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
// normal constructor

template <class T>
RayleighNoiseGenerator<T>::RayleighNoiseGenerator( char* instance_name,
                                              PracSimModel* outer_model,
                                              Signal<T>* noise_sig)
                      :PracSimModel(instance_name,
                                    outer_model)
{
  MODEL_NAME(RayleighNoiseGenerator);

  Noise_Sig = noise_sig;

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Seed);
  GET_FLOAT_PARM(Noise_Sigma);

  //----------------------------------------------
  MAKE_OUTPUT(Noise_Sig);

}
//=============================================
template <class T>
RayleighNoiseGenerator<T>::~RayleighNoiseGenerator( void ){ };
//===========================================
template <class T>
void RayleighNoiseGenerator<T>::Initialize(void)
{
  *DebugFile << "Now in RayleighNoiseGenerator::Initialize()" << endl;

  Proc_Block_Size = Noise_Sig->GetBlockSize();
}
//=============================================
template <class T>
int RayleighNoiseGenerator<T>::Execute(void)
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
    RayleighRandom(&seed, &rand_var);

    noise_sig_val = noise_sigma * rand_var;

    *noise_sig_ptr++ = noise_sig_val;
    }// end of main loop

  // put back variables that have changed
  Seed = seed;
  Noise_Sig->SetValidBlockSize(Proc_Block_Size);

  //----------------------------------------------

  return(_MES_AOK);
};

template RayleighNoiseGenerator<float>;
