//
//  File = quadcmpx.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "quadcmpx.h"
#include "model_graph.h"
#include "sigstuff.h"
#include "syst_graph.h"
#include "misdefs.h"
#include "gensig.h"
extern ParmFile ParmInput;
extern SystemGraph CommSystGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

QuadratureToComplex::QuadratureToComplex( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float>* i_in_sig,
                                Signal<float>* q_in_sig,
                                Signal< std::complex<float> >* out_sig)
              :PracSimModel(instance_name,
                            outer_model)
{
  I_In_Sig = i_in_sig;
  Q_In_Sig = q_in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT(Out_Sig);

  MAKE_INPUT(I_In_Sig);
  MAKE_INPUT(Q_In_Sig);

  SAME_RATE(I_In_Sig, Out_Sig);
  SAME_RATE(Q_In_Sig, Out_Sig);
}
//=============================================
QuadratureToComplex::~QuadratureToComplex( void ){ };
//===========================================
void QuadratureToComplex::Initialize(void)
{
  *DebugFile << "Now in QuadratureToComplex::Initialize()" << endl;

  Proc_Block_Size = Out_Sig->GetBlockSize();

}
//=============================================
int QuadratureToComplex::Execute(void)
{
  float *i_in_sig, *q_in_sig;
  std::complex<float> *out_sig, out_val;
  int is;
  int block_size;

  *DebugFile << "In QuadratureToComplex::Execute\0" << endl;
  out_sig = GET_OUTPUT_PTR(Out_Sig);
  i_in_sig = GET_INPUT_PTR(I_In_Sig);
  q_in_sig = GET_INPUT_PTR(Q_In_Sig);
  block_size = I_In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  for (is=0; is<block_size; is++)
    {
    *out_sig = std::complex<float>(*i_in_sig, *q_in_sig);
    out_sig++;
    i_in_sig++;
    q_in_sig++;
    }
  return(_MES_AOK);
}

