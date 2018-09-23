//
//  File = cpxtophs.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "cpxtoquad.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
extern ParmFile ParmInput;
extern SignalPlotter SigPlot;
extern int PassNumber;

//======================================================

CmpxToQuadrature::CmpxToQuadrature( char* instance_name,
                          PracSimModel* outer_model,
                          Signal< std::complex<float> >* in_signal,
                          Signal<float>* i_out_signal,
                          Signal<float>* q_out_signal)
            :PracSimModel(instance_name,
                          outer_model)
{
  In_Sig = in_signal;
  I_Out_Sig = i_out_signal;
  Q_Out_Sig = q_out_signal;

  MAKE_OUTPUT(I_Out_Sig);
  MAKE_OUTPUT(Q_Out_Sig);
  MAKE_INPUT(In_Sig);

  SAME_RATE(In_Sig, I_Out_Sig);
  SAME_RATE(In_Sig, Q_Out_Sig);
}
CmpxToQuadrature::~CmpxToQuadrature( void ){ };

void CmpxToQuadrature::Initialize(void)
{
  //Block_Size = In_Sig->GetBlockSize();
}

int CmpxToQuadrature::Execute()
{
  std::complex<float> *in_sig;
  float *i_out_sig;
  float *q_out_sig;
  int is;
  int block_size;

  i_out_sig = GET_OUTPUT_PTR(I_Out_Sig);
  q_out_sig = GET_OUTPUT_PTR(Q_Out_Sig);
  in_sig = GET_INPUT_PTR(In_Sig);
  block_size = In_Sig->GetValidBlockSize();
  I_Out_Sig->SetValidBlockSize(block_size);
  Q_Out_Sig->SetValidBlockSize(block_size);

  for (is=0; is<block_size; is++)
    {
    *i_out_sig++ = in_sig->real();
    *q_out_sig++ = in_sig->imag();
    in_sig++;
    }
  return(_MES_AOK);
}

