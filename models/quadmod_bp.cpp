//
//  File = quadmod_bp.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "quadmod_bp.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
extern ParmFile *ParmInput;

//========================================================================

QuadBandpassModulator::QuadBandpassModulator( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* i_in_sig,
                              Signal<float>* q_in_sig,
                              Signal<float>* out_sig)
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QuadBandpassModulator);
  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Phase_Unbal);
  GET_DOUBLE_PARM(Amp_Unbal);
  GET_DOUBLE_PARM(Carrier_Freq);
  GET_DOUBLE_PARM(Phase_Offset_Deg);
  Carrier_Freq_Rad = TWO_PI * Carrier_Freq;
  Phase_Unbal_Rad = PI * Phase_Unbal / 180.0;
  Phase_Offset_Rad = PI * Phase_Offset_Deg / 180.0;

  //-------------------------------------
  //  Connect input and output signals

  I_In_Sig = i_in_sig;
  Q_In_Sig = q_in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( I_In_Sig );
  MAKE_INPUT( Q_In_Sig );

}
//===========================================
QuadBandpassModulator::~QuadBandpassModulator( void ){ };
//===========================================

void QuadBandpassModulator::Initialize(void)
{
  Block_Size = I_In_Sig->GetBlockSize();
  Samp_Intvl = Out_Sig->GetSampIntvl();
  Time = 0.0;
}

//============================================
int QuadBandpassModulator::Execute(void)
{
  float *i_in_sig_ptr, *q_in_sig_ptr;
  float i_out, q_out;
  float *out_sig_ptr;
  double omega, time, time_base;
  double theta;
  double amp_unbal, phase_unbal_rad;
  double samp_intvl;
  int is;
  int block_size;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  i_in_sig_ptr = GET_INPUT_PTR( I_In_Sig );
  q_in_sig_ptr = GET_INPUT_PTR( Q_In_Sig );
  block_size = I_In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  omega = Carrier_Freq_Rad;
  time_base = Time;
  samp_intvl = Samp_Intvl;
  amp_unbal = Amp_Unbal;
  phase_unbal_rad = Phase_Unbal_Rad;
  theta = Phase_Offset_Rad;

  for (is=0; is<block_size; is++)
    {
    time = time_base + is * samp_intvl;
    i_out = (*i_in_sig_ptr) * cos(omega * time + theta);
    q_out = (*q_in_sig_ptr) * amp_unbal * sin(omega * time + phase_unbal_rad + theta);
    *out_sig_ptr = i_out - q_out;

    out_sig_ptr++;
    i_in_sig_ptr++;
    q_in_sig_ptr++;
    }
  Time = time_base + samp_intvl * Block_Size;
  return(_MES_AOK);
}

