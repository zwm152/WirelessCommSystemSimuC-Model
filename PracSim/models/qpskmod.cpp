//
//  File = qpskmod.cpp
//

#include <stdlib.h>
#include <fstream.h>
#include "parmfile.h"
#include "qpskmod.h"
#include "misdefs.h"
#include "modgraph.h"
#include "typedefs.h"
#include "cmpxsig.h"
extern ParmFile *ParmInput;

//======================================================

QpskModulator::QpskModulator( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<float>* i_in_sig,
                              Signal<float>* q_in_sig,
                              Signal< std::complex<float> >* cmpx_out_sig,
                              Signal<float>* mag_out_sig,
                              Signal<float>* phase_out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QpskModulator);

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Phase_Unbal);
  GET_DOUBLE_PARM(Amp_Unbal);
  //GET_INT_PARM("block_size\0");
  //GET_DOUBLE_PARM("samp_intvl\0");

  //-------------------------------------
  //  Connect input and output signals

  I_In_Sig = i_in_sig;
  Q_In_Sig = q_in_sig;
  Cmpx_Out_Sig = cmpx_out_sig;
  Mag_Out_Sig = mag_out_sig;
  Phase_Out_Sig = phase_out_sig;

  MAKE_OUTPUT( Cmpx_Out_Sig );
  MAKE_OUTPUT( Mag_Out_Sig );
  MAKE_OUTPUT( Phase_Out_Sig );
  MAKE_INPUT( I_In_Sig );
  MAKE_INPUT( Q_In_Sig );


  //-----------------------------------------
  // Set up derived parms

  double phase_unbal_rad = PI * Phase_Unbal / 180.0;
  Real_Unbal = float(cos(phase_unbal_rad) * Amp_Unbal);
  Imag_Unbal = float(sin(phase_unbal_rad) * Amp_Unbal);
}
//===========================================
QpskModulator::~QpskModulator( void ){ };
//===========================================

void QpskModulator::Initialize(void)
{
  Block_Size = I_In_Sig->GetBlockSize();
}

//============================================
int QpskModulator::Execute(void)
{
  float *i_in_sig_ptr, *q_in_sig_ptr;
  float i_out, q_out;
  float *phase_out_sig_ptr, *mag_out_sig_ptr;
  double real_unbal, imag_unbal;
  std::complex<float> *cmpx_out_sig_ptr;
  int is;

  cmpx_out_sig_ptr = GET_OUTPUT_PTR( Cmpx_Out_Sig );
  phase_out_sig_ptr = GET_OUTPUT_PTR( Phase_Out_Sig );
  mag_out_sig_ptr = GET_OUTPUT_PTR( Mag_Out_Sig );
  i_in_sig_ptr = GET_INPUT_PTR( I_In_Sig );
  q_in_sig_ptr = GET_INPUT_PTR( Q_In_Sig );

  real_unbal = Real_Unbal;
  imag_unbal = Imag_Unbal;
  //cout << "real_unbal = " << real_unbal << endl;
  //cout << "imag_unbal = " << imag_unbal << endl;

  for (is=0; is<Block_Size; is++)
    {
    i_out = (*i_in_sig_ptr) - (imag_unbal * (*q_in_sig_ptr));
    q_out = real_unbal * (*q_in_sig_ptr);
    //i_out *= 1.4;
    //q_out *= 1.4;
    (*cmpx_out_sig_ptr) = std::complex<float>( i_out, q_out);
    (*phase_out_sig_ptr) = 180.0*atan2(q_out, i_out)/PI;
    (*mag_out_sig_ptr) = sqrt(i_out*i_out + q_out*q_out);
    i_in_sig_ptr++;
    q_in_sig_ptr++;
    cmpx_out_sig_ptr++;
    phase_out_sig_ptr++;
    mag_out_sig_ptr++;
    }
  return(_MES_AOK);
}

