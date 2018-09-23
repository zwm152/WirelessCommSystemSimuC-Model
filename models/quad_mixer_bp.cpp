//
//  File = quad_mixer_bp.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "quad_mixer_bp.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

QuadBandpassMixer::QuadBandpassMixer( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< float >* in_sig,
                                  Signal< float >* i_ref_sig,
                                  Signal< float >* q_ref_sig,
                                  Signal< float >* i_out_sig,
                                  Signal< float >* q_out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QuadBandpassMixer);

  //-----------------------------------------
  //  Read model config parms

  //OPEN_PARM_BLOCK;


  //--------------------------------------
  //  Connect input and output signals

  I_Out_Sig = i_out_sig;
  Q_Out_Sig = q_out_sig;
  In_Sig = in_sig;
  I_Ref_Sig = i_ref_sig;
  Q_Ref_Sig = q_ref_sig;

  MAKE_OUTPUT( I_Out_Sig );
  MAKE_OUTPUT( Q_Out_Sig );
  MAKE_INPUT( In_Sig );
  MAKE_INPUT( I_Ref_Sig );
  MAKE_INPUT( Q_Ref_Sig );

}
//==============================================
QuadBandpassMixer::~QuadBandpassMixer( void ){ };
//==============================================

void QuadBandpassMixer::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
}

//============================================
int QuadBandpassMixer::Execute()
{
  float *i_out_sig_ptr;
  float *q_out_sig_ptr;
  float *in_sig_ptr;
  float *i_ref_sig_ptr;
  float *q_ref_sig_ptr;
  float in_val, i_ref_val, q_ref_val;
  float i_out_val, q_out_val;
  int is;

  #ifdef _DEBUG
    *DebugFile << "In QuadBandpassMixer::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  i_ref_sig_ptr = GET_INPUT_PTR( I_Ref_Sig );
  q_ref_sig_ptr = GET_INPUT_PTR( Q_Ref_Sig );
  i_out_sig_ptr = GET_OUTPUT_PTR( I_Out_Sig );
  q_out_sig_ptr = GET_OUTPUT_PTR( Q_Out_Sig );

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;
    i_ref_val = *i_ref_sig_ptr++;
    q_ref_val = *q_ref_sig_ptr++;

    i_out_val = in_val * i_ref_val;
    q_out_val = in_val * q_ref_val;

    *i_out_sig_ptr++ = i_out_val;
    *q_out_sig_ptr++ = q_out_val;
    }
  return(_MES_AOK);
}

