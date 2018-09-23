//
//  File = quaddem.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "quaddem.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
extern ParmFile *ParmInput;

//========================================================================
// constructor 

QuadratureDemod::QuadratureDemod( char* instance_name,
                              PracSimModel* outer_model,
                                  Signal< std::complex< float > >* in_sig,
                                  Signal< std::complex< float > >* carrier_ref_sig,
                                  Signal< float >* i_wave_out,
                                  Signal< float >* q_wave_out )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QuadratureDemod);

  //--------------------------------------
  //  Connect input and output signals

  I_Wave_Out = i_wave_out;
  Q_Wave_Out = q_wave_out;
  In_Sig = in_sig;
  Carrier_Ref_Sig = carrier_ref_sig;

  MAKE_OUTPUT( I_Wave_Out );
  MAKE_OUTPUT( Q_Wave_Out );
  MAKE_INPUT( In_Sig );
  MAKE_INPUT( Carrier_Ref_Sig );
}
//===========================================
QuadratureDemod::~QuadratureDemod( void ){ };
//===========================================

void QuadratureDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
}

//============================================
int QuadratureDemod::Execute(void)
{
  float *i_wave_out_ptr, *q_wave_out_ptr;
  std::complex<float> *in_sig_ptr;
  std::complex<float> *carrier_ref_sig_ptr;
  std::complex<float> in_val, carrier_val, correl_ref;
  int is;

  I_Wave_Out->SetValidBlockSize(Block_Size);
  Q_Wave_Out->SetValidBlockSize(Block_Size);

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  carrier_ref_sig_ptr = GET_INPUT_PTR( Carrier_Ref_Sig );
  i_wave_out_ptr = GET_OUTPUT_PTR( I_Wave_Out );
  q_wave_out_ptr = GET_OUTPUT_PTR( Q_Wave_Out );

  std::complex<float> cmpx_j = std::complex<float>(0.0,-1.0);

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;
    carrier_val = *carrier_ref_sig_ptr++;

    //  mix with I subcarrier
    correl_ref = std::conj(carrier_val);
    *i_wave_out_ptr++ = std::real(in_val * correl_ref);

    //  mix with Q subcarrier
    correl_ref *= cmpx_j;
    *q_wave_out_ptr++ = std::real(in_val * correl_ref);

    }
  return(_MES_AOK);
}

