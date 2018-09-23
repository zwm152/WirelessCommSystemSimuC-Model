//
//  File = carrier_genie.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "misdefs.h"
#include "carrier_genie.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

CarrierRecoveryGenie::CarrierRecoveryGenie( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float> *recov_carrier )
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(CarrierRecoveryGenie);

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Carrier_Freq);
  Carrier_Freq_Rad = TWO_PI * Carrier_Freq;

  //-----------------------------------
  //  Signals

  Recov_Carrier = recov_carrier;

  MAKE_OUTPUT( Recov_Carrier );

  return;
  }
//================================================
CarrierRecoveryGenie::~CarrierRecoveryGenie( void ){ };

//=======================================================

void CarrierRecoveryGenie::Initialize()
{
  //---------------------------------------
  //  Initialize derived parameters
  Block_Size = Recov_Carrier->GetBlockSize();
  Samp_Intvl = Recov_Carrier->GetSampIntvl();
  Time = 0.0;
}

//=======================================
int CarrierRecoveryGenie::Execute()
{
  float *recov_carrier_ptr;
  double omega, time, time_base;
  double samp_intvl;
  int is;

  recov_carrier_ptr = GET_OUTPUT_PTR( Recov_Carrier );
  omega = Carrier_Freq_Rad;
  time_base = Time;
  samp_intvl = Samp_Intvl;

  for(is=0; is<Block_Size; is++)
    {
    time = time_base + is * samp_intvl;
    *recov_carrier_ptr++ = cos(omega * time);
    }
  Time = time_base + samp_intvl * Block_Size;
  return(_MES_AOK);

}

