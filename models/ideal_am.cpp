//
//  File = ideal_am.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "ideal_am.h"
#include "model_graph.h"
//#include "uni_rand.h"
extern ParmFile *ParmInput;

//======================================================

IdealAmplitudeModulator::IdealAmplitudeModulator( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* carrier_sig,
                        Signal<float>* mod_sig,
                        Signal<float>* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(IdealAmplitudeModulator);

  //ENABLE_MULTIRATE;

  //---------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;
  GET_DOUBLE_PARM( Modulation_Index );

  //--------------------------------------
  //  Connect input and output signals

  Carrier_Sig = carrier_sig;
  Mod_Sig = mod_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );

  MAKE_INPUT( Carrier_Sig );
  MAKE_INPUT( Mod_Sig );

  }

//======================================
IdealAmplitudeModulator::~IdealAmplitudeModulator( void ){ };

//=======================================
void IdealAmplitudeModulator::Initialize(void)
{

  //------------------

  double samp_intvl = Out_Sig->GetSampIntvl();

  Out_Avg_Block_Size = Out_Sig->GetBlockSize();

}
//=======================================================
int IdealAmplitudeModulator::Execute()
{
  float *out_sig_ptr;
  float out_sig_val;
  float *carrier_sig_ptr;
  float *mod_sig_ptr;
  int is;
  int block_size;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
  carrier_sig_ptr = GET_INPUT_PTR( Carrier_Sig );
  mod_sig_ptr = GET_INPUT_PTR( Mod_Sig );

  block_size = Mod_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  for (is=0; is<block_size; is++) {

   out_sig_val = (1.0 + Modulation_Index*(*mod_sig_ptr++)) 
               * (*carrier_sig_ptr++);
   *out_sig_ptr++ = out_sig_val;
  }
  return(_MES_AOK);
}

