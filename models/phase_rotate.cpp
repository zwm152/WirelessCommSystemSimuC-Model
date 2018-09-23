//
//  File = phase_rotate.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "misdefs.h"
#include "model_error.h"
#include "phase_rotate.h"
#include "model_graph.h"
#include "sinc.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//======================================================

PhaseRotate::PhaseRotate( char* instance_name,
                        PracSimModel* outer_model,
                        Signal< std::complex<float> >* in_sig,
                        Signal< std::complex<float> >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(PhaseRotate);

  //---------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;
  GET_FLOAT_PARM( Phase_Shift_Deg );

  Rotate_Val = std::complex<float>( float(cos(PI*Phase_Shift_Deg/180.0)), 
                                    float(sin(PI*Phase_Shift_Deg/180.0)));

  //--------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  }

//======================================
PhaseRotate::~PhaseRotate( void ){ };

//=======================================
void PhaseRotate::Initialize(void)
{

  //------------------
  Block_Size = Out_Sig->GetBlockSize();

}
//=======================================================
int PhaseRotate::Execute()
{
  std::complex<float> *out_sig_ptr;
  std::complex<float> *in_sig_ptr;
  int is;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  //-------------------------------------------------------
  //  compute outputs that use inputs from previous block

  for (is=0; is<Block_Size; is++)
    {
    *out_sig_ptr++ = Rotate_Val * (*in_sig_ptr++);
    }

  return(_MES_AOK);
}

