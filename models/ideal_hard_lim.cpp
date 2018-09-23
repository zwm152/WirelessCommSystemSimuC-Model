//
//  File = ideal_hard_lim.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "ideal_hard_lim.h"
#include "model_graph.h"
//#include "uni_rand.h"
extern ParmFile *ParmInput;

//======================================================

IdealHardLimiter::IdealHardLimiter( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* in_sig,
                        Signal<float>* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(IdealHardLimiter);

  //ENABLE_MULTIRATE;

  //---------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;
  GET_DOUBLE_PARM( Pos_Limit );
  GET_DOUBLE_PARM( Neg_Limit );

  //--------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );

  MAKE_INPUT( In_Sig );

  }

//======================================
IdealHardLimiter::~IdealHardLimiter( void ){ };

//=======================================
void IdealHardLimiter::Initialize(void)
{

  //------------------

  double samp_intvl = Out_Sig->GetSampIntvl();

  Out_Avg_Block_Size = Out_Sig->GetBlockSize();

}
//=======================================================
int IdealHardLimiter::Execute()
{
  float *out_sig_ptr;
  float out_sig_val;
  float *in_sig_ptr;
  float in_sig_val;
  int is;
  int block_size;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  for (is=0; is<block_size; is++) {

      in_sig_val = *in_sig_ptr++;
      if(in_sig_val >= 0.0) {
         out_sig_val = Pos_Limit;
      }
      else {
         out_sig_val = Neg_Limit;
      }
      *out_sig_ptr++ = out_sig_val;
  }
  return(_MES_AOK);
}

