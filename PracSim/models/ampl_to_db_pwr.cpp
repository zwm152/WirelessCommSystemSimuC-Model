//
//  File = quad_to_polar.cpp
//

#include <stdlib.h>
//#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "ampl_to_db_pwr.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"
#include "misdefs.h"
//#include "gensig.h"
extern ParmFile ParmInput;
extern SystemGraph SystGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

AmplitudeToDbPower::AmplitudeToDbPower( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float>* in_sig,
                                Signal<float>* out_sig)
              :PracSimModel(instance_name,
                            outer_model)
{
  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT(Out_Sig);

  MAKE_INPUT(In_Sig);

  SAME_RATE(In_Sig, Out_Sig);
}
//=============================================
AmplitudeToDbPower::~AmplitudeToDbPower( void ){ };
//===========================================
void AmplitudeToDbPower::Initialize(void)
{
  *DebugFile << "Now in AmplitudeToDbPower::Initialize()" << endl;

  Proc_Block_Size = Out_Sig->GetBlockSize();

}
//=============================================
int AmplitudeToDbPower::Execute(void)
{
  float *in_sig;
  float *out_sig;
  double in_val;
  int is;

  *DebugFile << "In AmplitudeToDbPower::Execute\0" << endl;
  out_sig = GET_OUTPUT_PTR(Out_Sig);
  in_sig = GET_INPUT_PTR(In_Sig);

  Out_Sig->SetValidBlockSize(Proc_Block_Size);

  for (is=0; is<Proc_Block_Size; is++)
    {
    in_val = *in_sig++;
    if(fabs(in_val)<1e-100) {
      *out_sig = -100.0;
    }
    else {
      *out_sig = float(10.0*log10(in_val*in_val));
   }
    out_sig++;
    }
  return(_MES_AOK);
}

