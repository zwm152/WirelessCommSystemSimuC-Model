//
//  File = mixer_bp.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "mixer_bp.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

BandpassMixer::BandpassMixer( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< float >* in_sig_1,
                                  Signal< float >* in_sig_2,
                                  Signal< float >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(BandpassMixer);

  //-----------------------------------------
  //  Read model config parms

  //OPEN_PARM_BLOCK;


  //--------------------------------------
  //  Connect input and output signals

  Out_Sig = out_sig;
  In_Sig_1 = in_sig_1;
  In_Sig_2 = in_sig_2;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig_1 );
  MAKE_INPUT( In_Sig_2 );

}
//==============================================
BandpassMixer::~BandpassMixer( void ){ };
//==============================================

void BandpassMixer::Initialize(void)
{
  Block_Size = In_Sig_1->GetBlockSize();
}

//============================================
int BandpassMixer::Execute()
{
  float *out_sig_ptr;
  float *in_sig_1_ptr;
  float *in_sig_2_ptr;
  float in_val_1, in_val_2;
  float out_val;
  int is;
  int block_size;

  #ifdef _DEBUG
    *DebugFile << "In QuadBandpassMixer::Execute\0" << endl;
  #endif

   block_size = In_Sig_1->GetValidBlockSize();
  in_sig_1_ptr = GET_INPUT_PTR( In_Sig_1 );
  in_sig_2_ptr = GET_INPUT_PTR( In_Sig_2 );
  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
  Out_Sig->SetValidBlockSize(block_size);

  for (is=0; is<block_size; is++)
    {
    in_val_1 = *in_sig_1_ptr++;
    in_val_2 = *in_sig_2_ptr++;

    out_val = in_val_1 * in_val_2;

    *out_sig_ptr++ = out_val;
    }
  return(_MES_AOK);
}

