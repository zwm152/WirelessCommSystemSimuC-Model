//
//  File = bpsk_mod.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bpsk_mod.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
//#include "cmpxsig.h"
extern ParmFile *ParmInput;

//========================================================================
// constructor

BpskModulator::BpskModulator( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* in_sig,
                              Signal< std::complex<float> >* cmpx_out_sig)
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(BpskModulator);

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Carrier_Phase_Deg);

  //-------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Cmpx_Out_Sig = cmpx_out_sig;

  MAKE_OUTPUT( Cmpx_Out_Sig );
  MAKE_INPUT( In_Sig );

  //----------------------------------
  //  Compute derived parameters

  Carrier_Phase_Rad = PI * Carrier_Phase_Deg / 180.0;

}
//========================================================
// destructor

BpskModulator::~BpskModulator( void ){ };


//===========================================

void BpskModulator::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Cmpx_Out_Sig->SetValidBlockSize(Block_Size);
}

//============================================
int BpskModulator::Execute(void)
{
  float *in_sig_ptr;
  float i_out, q_out;
  std::complex<float> *cmpx_out_sig_ptr;
  int is;

  cmpx_out_sig_ptr = GET_OUTPUT_PTR( Cmpx_Out_Sig );

  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  for (is=0; is<Block_Size; is++)
    {
    i_out = float((*in_sig_ptr) * cos(Carrier_Phase_Rad));
    q_out = float((*in_sig_ptr) * sin(Carrier_Phase_Rad));
    *cmpx_out_sig_ptr++ = std::complex<float>( i_out, q_out);

    in_sig_ptr++;
    }
  return(_MES_AOK);
}

