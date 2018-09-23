//
//  File = fsk_genie_bp.cpp
//

#include <stdlib.h>
//#include <fstream>
//#include <strstream>
#include "parmfile.h"
#include "misdefs.h"
#include "fsk_genie_bp.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

FskBandpassCarrierGenie::FskBandpassCarrierGenie( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<float> *lo_ref_sig,
                                Signal<float> *hi_ref_sig )
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(FskBandpassCarrierGenie);

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Carrier_Freq_Hz);
  GET_DOUBLE_PARM(Freq_Offset_Hz);
  GET_INT_PARM( Lo_Ref_Samp_Shift );
  GET_INT_PARM( Hi_Ref_Samp_Shift );

  //-----------------------------------
  //  Signals

  Lo_Ref_Sig = lo_ref_sig;
  Hi_Ref_Sig = hi_ref_sig;

  MAKE_OUTPUT( Lo_Ref_Sig );
  MAKE_OUTPUT( Hi_Ref_Sig );
  //----------------------------------
  //  Compute derived parameters

  Freq_Hi_Hz = Carrier_Freq_Hz + Freq_Offset_Hz;
  Freq_Lo_Hz = Carrier_Freq_Hz - Freq_Offset_Hz;


  return;
  }
//================================================
FskBandpassCarrierGenie::~FskBandpassCarrierGenie( void ){ };

//=======================================================

void FskBandpassCarrierGenie::Initialize()
{
  //---------------------------------------
  //  Initialize derived parameters
  Block_Size = Lo_Ref_Sig->GetBlockSize();
  Samp_Intvl = Lo_Ref_Sig->GetSampIntvl();
  Cumul_Phase_Lo = Lo_Ref_Samp_Shift * Samp_Intvl * Freq_Lo_Hz;
  Cumul_Phase_Hi = Hi_Ref_Samp_Shift * Samp_Intvl * Freq_Hi_Hz;
}

//=======================================
int FskBandpassCarrierGenie::Execute()
{
  float *lo_ref_sig_ptr;
  float *hi_ref_sig_ptr;
  double cumul_phase_lo, cumul_phase_hi;
  double freq_lo_hz, freq_hi_hz;
  double samp_intvl;
  int is;

  lo_ref_sig_ptr = GET_OUTPUT_PTR( Lo_Ref_Sig );
  hi_ref_sig_ptr = GET_OUTPUT_PTR( Hi_Ref_Sig );

  cumul_phase_lo = Cumul_Phase_Lo;
  cumul_phase_hi = Cumul_Phase_Hi;
  freq_lo_hz = Freq_Lo_Hz;
  freq_hi_hz = Freq_Hi_Hz;

  samp_intvl = Samp_Intvl;

  for(is=0; is<Block_Size; is++)
    {
    cumul_phase_lo = fmod(cumul_phase_lo + samp_intvl * freq_lo_hz, 1.0);
    cumul_phase_hi = fmod(cumul_phase_hi + samp_intvl * freq_hi_hz, 1.0);
    *lo_ref_sig_ptr++ = float(sin(TWO_PI * cumul_phase_lo));
    *hi_ref_sig_ptr++ = float(sin(TWO_PI * cumul_phase_hi));
    }
  Cumul_Phase_Lo = cumul_phase_lo;
  Cumul_Phase_Hi = cumul_phase_hi;
  return(_MES_AOK);

}

