//
//  File = cpxtophs.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "cpxtophs.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
extern ParmFile ParmInput;
extern SignalPlotter SigPlot;
extern int PassNumber;

//======================================================

CmpxToPhase::CmpxToPhase( char* instance_name,
                          PracSimModel* outer_model,
                          Signal< std::complex<float> >* in_signal,
                          Signal<float>* out_signal,
                          Signal<float>* mag_signal)
            :PracSimModel(instance_name,
                          outer_model)
{
  In_Sig = in_signal;
  Out_Sig = out_signal;
  Mag_Sig = mag_signal;

  MAKE_OUTPUT(Out_Sig);
  MAKE_OUTPUT(Mag_Sig);
  MAKE_INPUT(In_Sig);

  SAME_RATE(In_Sig, Out_Sig);
  SAME_RATE(In_Sig, Mag_Sig);
}
CmpxToPhase::~CmpxToPhase( void ){ };

void CmpxToPhase::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
}

int CmpxToPhase::Execute()
{
  std::complex<float> *in_sig;
  float *out_sig, *mag_sig;
  int is;

  Out_Sig->SetValidBlockSize(Block_Size);
  Mag_Sig->SetValidBlockSize(Block_Size);

  out_sig = GET_OUTPUT_PTR(Out_Sig);
  mag_sig = GET_OUTPUT_PTR(Mag_Sig);
  in_sig = GET_INPUT_PTR(In_Sig);

  for (is=0; is<Block_Size; is++)
    {
    *out_sig++ = std::arg(std::conj(*in_sig));
    *mag_sig++ = std::abs(*in_sig)/64.0;
    in_sig++;
    }
  return(_MES_AOK);
}

