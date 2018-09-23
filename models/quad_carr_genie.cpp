//
//  File = quad_carr_genie.cpp
//

#include <stdlib.h>
//#include <fstream>
//#include <strstream>
#include "parmfile.h"
#include "misdefs.h"
#include "quad_carr_genie.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

QuadCarrierRecovGenie::QuadCarrierRecovGenie( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float> *i_recov_carrier,
                                Signal<float> *q_recov_carrier )
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(QuadCarrierRecovGenie);

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Carrier_Freq);
  Carrier_Freq_Rad = TWO_PI * Carrier_Freq;
  GET_DOUBLE_PARM(Phase_Offset_Deg);
  Phase_Offset_Rad = PI * Phase_Offset_Deg /180.0;

  //-----------------------------------
  //  Signals

  I_Recov_Carrier = i_recov_carrier;
  Q_Recov_Carrier = q_recov_carrier;

  MAKE_OUTPUT( I_Recov_Carrier );
  MAKE_OUTPUT( Q_Recov_Carrier );

  return;
  }
//================================================
QuadCarrierRecovGenie::~QuadCarrierRecovGenie( void ){ };

//=======================================================

void QuadCarrierRecovGenie::Initialize()
{
  //---------------------------------------
  //  Initialize derived parameters
  Block_Size = I_Recov_Carrier->GetBlockSize();
  Samp_Intvl = I_Recov_Carrier->GetSampIntvl();
  Time = 0.0;
}

//=======================================
int QuadCarrierRecovGenie::Execute()
{
  float *i_recov_carrier_ptr;
  float *q_recov_carrier_ptr;
  double omega, time, time_base;
  double theta;
  double samp_intvl;
  int is;

  i_recov_carrier_ptr = GET_OUTPUT_PTR( I_Recov_Carrier );
  q_recov_carrier_ptr = GET_OUTPUT_PTR( Q_Recov_Carrier );

  omega = Carrier_Freq_Rad;
  theta = Phase_Offset_Rad;
  time_base = Time;
  samp_intvl = Samp_Intvl;

  for(is=0; is<Block_Size; is++)
    {
    time = time_base + is * samp_intvl;
    *i_recov_carrier_ptr++ = cos(omega * time + theta);
    *q_recov_carrier_ptr++ = -sin(omega * time + theta);
    }
  Time = time_base + samp_intvl * Block_Size;
  return(_MES_AOK);

}

