//
//  File = stepgen.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "stepgen.h"
//#include "uni_rand.h"
#include "parmfile.h"
#include "model_graph.h"

extern ParmFile *ParmInput;
extern int PassNumber;
//extern ActiveSystemGraph ActSystGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif


//======================================================

StepGener::StepGener( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* out_sig )
        :PracSimModel( instance_name,
                        outer_model )
{
  MODEL_NAME(StepGener);
  Out_Sig = out_sig;

  MAKE_OUTPUT(Out_Sig);
}
//====================================================
StepGener::~StepGener( void){};
//====================================================
void StepGener::Initialize(void)
{
  *DebugFile << "Now in StepGener::Initialize()" << endl;

  Block_Size = Out_Sig->GetBlockSize();
  //Samp_Rate = Out_Sig->GetSampRate();
}
//====================================================
int StepGener::Execute()
{
   int is;
   float *out_samps;

   //--------------------------------
   //  Get pointer for output buffer

   out_samps = GET_OUTPUT_PTR(Out_Sig);

   for (is=0; is<Block_Size; is++){
//      if((PassNumber > 1) || ( (PassNumber==1) && (is>=10)))
      if( (PassNumber==1) && (is==10)) {
         *out_samps = 1.0;
      }
      else {
         *out_samps = 0.0;
      }
      out_samps++;
   }
   Out_Sig->SetValidBlockSize(Block_Size);
   return(_MES_AOK);
}

