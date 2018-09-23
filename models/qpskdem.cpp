//
//  File = qpskdem.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "qpskdem.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;

//======================================================

QpskDemodulator::QpskDemodulator( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< std::complex< float > >* in_sig,
                                  Signal< float >* i_out_sig,
                                  Signal< float >* q_out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QpskDemodulator);
  

  //-----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Phase_Unbal);

  //int block_size = ParmInput.GetIntParm("block_size\0");
  //double samp_intvl = ParmInput.GetDoubleParm("samp_intvl\0");

  //--------------------------------------
  //  Connect input and output signals

  I_Out_Sig = i_out_sig;
  Q_Out_Sig = q_out_sig;
  In_Sig = in_sig;

  MAKE_OUTPUT( I_Out_Sig );
  MAKE_OUTPUT( Q_Out_Sig );
  MAKE_INPUT( In_Sig );

  //------------------
}
//==============================================
QpskDemodulator::~QpskDemodulator( void ){ };
//==============================================

void QpskDemodulator::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
}

//============================================
int QpskDemodulator::Execute()
{
  float *i_out_sig_ptr, *q_out_sig_ptr;
  double ref_phase, phase_unbal;
  std::complex<float> *in_sig_ptr, in_samp;
  int is;

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  i_out_sig_ptr = GET_OUTPUT_PTR( I_Out_Sig );
  q_out_sig_ptr = GET_OUTPUT_PTR( Q_Out_Sig );

  phase_unbal = Phase_Unbal;

  for (is=0; is<Block_Size; is++)
    {
    in_samp = *in_sig_ptr;
    //ref_phase = -0.78539816;
    //ref_phase = 1.5707963;
    ref_phase = 0.0;
    *i_out_sig_ptr = std::real(in_samp *
                  std::complex<float>( cos(ref_phase),
                                 sin(ref_phase)));
    *q_out_sig_ptr = std::imag(in_samp * 
                  std::complex<float>( cos(ref_phase + phase_unbal),
                                 sin(ref_phase + phase_unbal)));
    i_out_sig_ptr++;
    q_out_sig_ptr++;
    in_sig_ptr++;
    }
  return(_MES_AOK);
}

