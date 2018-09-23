//
//  File = phase_genie.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "misdefs.h"
#include "phase_genie.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

PhaseRecoveryGenie::PhaseRecoveryGenie( char* instance_name,
                                PracSimModel* outer_model,
                                Signal< std::complex<float> > *recov_carrier_sig )
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(PhaseRecoveryGenie);

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Carrier_Phase_Deg);
  Carrier_Phase = PI * Carrier_Phase_Deg / 180.0;
  Carrier_Val = std::complex<float>( float(cos(Carrier_Phase)), float(sin(Carrier_Phase)) );

  //-----------------------------------
  //  Signals

  Recov_Carrier_Sig = recov_carrier_sig;

  MAKE_OUTPUT( Recov_Carrier_Sig );

  return;
  }
//================================================
PhaseRecoveryGenie::~PhaseRecoveryGenie( void ){ };

//=======================================================

void PhaseRecoveryGenie::Initialize()
{
  Block_Size = Recov_Carrier_Sig->GetBlockSize();
  return;
}

//=======================================
int PhaseRecoveryGenie::Execute()
{
  int is;
  std::complex<float> carrier_val;
  std::complex<float> *recov_carrier_sig_ptr;
  Recov_Carrier_Sig->SetValidBlockSize(Block_Size);

  carrier_val = Carrier_Val;
  recov_carrier_sig_ptr = GET_OUTPUT_PTR( Recov_Carrier_Sig );

  for(is=0; is<Block_Size; is++)
    {
    *recov_carrier_sig_ptr++ = carrier_val;
    }
  return(_MES_AOK);

}

