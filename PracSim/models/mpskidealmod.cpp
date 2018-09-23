//
//  File = mpskidealmod.cpp
//

#include <stdlib.h>
//#include <fstream.>
#include "parmfile.h"
#include "mpskidealmod.h"
#include "model_error.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

MpskIdealModulator::MpskIdealModulator( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<byte_t>* in_sig,
                              Signal< std::complex<float> >* cmpx_out_sig,
                              Signal<bit_t>* symb_clock_out,
                              Signal<float>* mag_out_sig,
                              Signal<float>* phase_out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(MpskIdealModulator);

  ENABLE_MULTIRATE;

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Bits_Per_Symb);
  GET_INT_PARM(Samps_Per_Symb);
  GET_DOUBLE_PARM(Phase_Unbal);
  GET_DOUBLE_PARM(Amp_Unbal);
  GET_DOUBLE_PARM(Symb_Duration);

  //-------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Cmpx_Out_Sig = cmpx_out_sig;
  Symb_Clock_Out = symb_clock_out;
  Mag_Out_Sig = mag_out_sig;
  Phase_Out_Sig = phase_out_sig;

  MAKE_OUTPUT( Cmpx_Out_Sig );
  MAKE_OUTPUT( Mag_Out_Sig );
  MAKE_OUTPUT( Phase_Out_Sig );
  MAKE_OUTPUT( Symb_Clock_Out );
  MAKE_INPUT( In_Sig );

  // one sample per symbol from input
  double resamp_rate = double(Samps_Per_Symb);

  SET_SAMP_INTVL( In_Sig, Symb_Duration);
  CHANGE_RATE( In_Sig, Symb_Clock_Out, resamp_rate );
  CHANGE_RATE( In_Sig, Cmpx_Out_Sig, resamp_rate );
  CHANGE_RATE( In_Sig, Mag_Out_Sig, resamp_rate );
  CHANGE_RATE( In_Sig, Phase_Out_Sig, resamp_rate );


  //-----------------------------------------
  // Set up derived parms

  double phase_unbal_rad = PI * Phase_Unbal / 180.0;
  Real_Unbal = float(cos(phase_unbal_rad) * Amp_Unbal);
  Imag_Unbal = float(sin(phase_unbal_rad) * Amp_Unbal);

  Num_Diff_Symbs = 1;
  for(int i=1; i<=Bits_Per_Symb; i++)
    Num_Diff_Symbs *=2;

  //------------------------------------------------------------
  //  generate tables of I & Q values for desired constellation
  //  (someday add capability to read intables from parm file)
  //  (this so we can do Gray coding, etc.)

  double angle;
  double magnitude = 1.0;
  float work;
  I_Compon = new float[Num_Diff_Symbs];
  Q_Compon = new float[Num_Diff_Symbs];

  for(int isymb=0; isymb<Num_Diff_Symbs; isymb++)
    {
    angle = isymb * TWO_PI/double(Num_Diff_Symbs);
    work = float(magnitude * cos(angle));
    I_Compon[isymb] = work;
    work = float(magnitude * sin(angle));
    Q_Compon[isymb] = work;
    }

}
//===========================================
MpskIdealModulator::~MpskIdealModulator( void ){ };
//===========================================

void MpskIdealModulator::Initialize(void)
{
  In_Block_Size = In_Sig->GetBlockSize();
  Out_Block_Size = Cmpx_Out_Sig->GetBlockSize();

}

//============================================
int MpskIdealModulator::Execute(void)
{
  byte_t *in_sig_ptr;
  float i_out, q_out;
  float i_comp, q_comp;
  float phase_out, mag_out;
  float *phase_out_sig_ptr, *mag_out_sig_ptr;
  double real_unbal, imag_unbal;
  std::complex<float> *cmpx_out_sig_ptr;
  std::complex<float> cmpx_out;
  bit_t *symb_clock_out_ptr;
  int is;
  #ifdef _DEBUG
    *DebugFile << "In MpskIdealModulator::Execute\0" << endl;
  #endif

  cmpx_out_sig_ptr = GET_OUTPUT_PTR( Cmpx_Out_Sig );
  phase_out_sig_ptr = GET_OUTPUT_PTR( Phase_Out_Sig );
  mag_out_sig_ptr = GET_OUTPUT_PTR( Mag_Out_Sig );
  symb_clock_out_ptr = GET_OUTPUT_PTR( Symb_Clock_Out );
  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  real_unbal = Real_Unbal;
  imag_unbal = Imag_Unbal;
  //cout << "real_unbal = " << real_unbal << endl;
  //cout << "imag_unbal = " << imag_unbal << endl;

  for (is=0; is<In_Block_Size; is++)
    {
    //  get input symbol and split into ideal I & Q components
    i_comp = I_Compon[*in_sig_ptr];
    q_comp = Q_Compon[*in_sig_ptr];
    in_sig_ptr++;

    i_out = i_comp - (imag_unbal * q_comp);
    q_out = real_unbal * q_comp;
    cmpx_out = std::complex<float>( i_out, q_out);
    phase_out = 180.0 * std::arg<float>(cmpx_out)/PI;
    mag_out = sqrt(i_out*i_out + q_out*q_out);

    for(int isamp=0; isamp<Samps_Per_Symb; isamp++)
      {
      *cmpx_out_sig_ptr++ = cmpx_out;
      *phase_out_sig_ptr++ = phase_out;
      *mag_out_sig_ptr++ = mag_out;

      if(isamp == (Samps_Per_Symb-1))
        *symb_clock_out_ptr++ = 1;
      else
        *symb_clock_out_ptr++ = 0;
      }
    }
  return(_MES_AOK);
}

