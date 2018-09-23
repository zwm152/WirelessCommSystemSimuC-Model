//
//  File = ar_proc_gen.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "ar_proc_gen.h"
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

template <class T>
ArProcessGenerator<T>::ArProcessGenerator( char* instance_name,
                                              PracSimModel* outer_model,
                                              Signal<T>* noise_sig)
                      :PracSimModel(instance_name,
                                    outer_model)
{
  MODEL_NAME(ArProcessGenerator);

  Noise_Sig = noise_sig;

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Noise_Seed);
  GET_DOUBLE_PARM(Driving_Variance);
  Driving_Sigma = float(sqrt(Driving_Variance));
  GET_INT_PARM(Ar_Order);
  int ar_size = Ar_Order+1;
  A_Coeffs = new double[ar_size];
  GET_DOUBLE_PARM_ARRAY(A_Coeffs, ar_size);

  //----------------------------------------------
  MAKE_OUTPUT(Noise_Sig);
  Old_Output = new T[Ar_Order];
  for(int i=0; i<Ar_Order; i++) Old_Output[i] = 0.0;

}
//=============================================
template <class T>
ArProcessGenerator<T>::~ArProcessGenerator( void ){ };
//===========================================
template <class T>
void ArProcessGenerator<T>::Initialize(void)
{
  *DebugFile << "Now in ArProcessGenerator::Initialize()" << endl;

  Proc_Block_Size = Noise_Sig->GetBlockSize();
}
//=============================================
template <class T>
int ArProcessGenerator<T>::Execute(void)
{
   int is, idx;
   float *noise_sig_ptr;
   float driving_sigma;
   float rand_var;
   float noise_sig_val;

   //--------------------------------------------
   driving_sigma = Driving_Sigma;
   long noise_seed = Noise_Seed;

   //---------------------------------------------

   noise_sig_ptr = GET_OUTPUT_PTR(Noise_Sig);

   //-------------------------------------------------
   //  main loop

   for(is=0; is<Proc_Block_Size; is++)
   {
      // generate gaussian RV
      GaussRandom(&noise_seed, &rand_var);
      noise_sig_val = driving_sigma * rand_var;

      for(idx=0; idx<Ar_Order; idx++)
      {
         noise_sig_val -= A_Coeffs[idx+1]*Old_Output[idx];
      }
      for(idx=Ar_Order-1; idx>0; idx--)
      {
         Old_Output[idx] = Old_Output[idx-1];
      }

      Old_Output[0] = noise_sig_val;
      *noise_sig_ptr++ = noise_sig_val;
   }// end of main loop

   // put back variables that have changed
   Noise_Seed = noise_seed;

   //----------------------------------------------
   Noise_Sig->SetValidBlockSize(Proc_Block_Size);
   return(_MES_AOK);
};

template ArProcessGenerator<float>;
