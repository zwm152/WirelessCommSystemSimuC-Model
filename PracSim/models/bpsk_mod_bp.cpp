//
//  File = bpsk_mod_bp.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bpsk_mod_bp.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
//#include "cmpxsig.h"
extern ParmFile *ParmInput;

//========================================================================
// constructor

BpskBandpassModulator::BpskBandpassModulator( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* in_sig,
                              Signal<float>* out_sig)
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(BpskBandpassModulator);

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Carrier_Freq);

  //-------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  //----------------------------------
  //  Compute derived parameters

  Carrier_Freq_Rad = TWO_PI * Carrier_Freq;

}
//========================================================
// destructor

BpskBandpassModulator::~BpskBandpassModulator( void ){ };


//===========================================

void BpskBandpassModulator::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  Time = 0.0;
}

//============================================
int BpskBandpassModulator::Execute(void)
{
  float *in_sig_ptr;
  float *out_sig_ptr;
  double omega, time, time_base;
  double samp_intvl;
  int is;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  Block_Size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(Block_Size);

  omega = Carrier_Freq_Rad;
  time_base = Time;
  samp_intvl = Samp_Intvl;

  for (is=0; is<Block_Size; is++)
    {
    time = time_base + is * samp_intvl;
    *out_sig_ptr++ = (*in_sig_ptr) * cos( (PI/2.0) + (omega * time));

    in_sig_ptr++;
    }
  Time = time_base + samp_intvl * Block_Size;
  return(_MES_AOK);
}

