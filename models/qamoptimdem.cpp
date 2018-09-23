//
//  File = qamoptimdem.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "qamoptimdem.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

QamOptimalDemod::QamOptimalDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< std::complex< float > >* in_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< byte_t >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QamOptimalDemod);

  ENABLE_MULTIRATE;

  //-----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Bits_Per_Symb);
  GET_INT_PARM(Samps_Per_Symb);

  //--------------------------------------
  //  Connect input and output signals

  Out_Sig = out_sig;
  Symb_Clock_In = symb_clock_in;
  In_Sig = in_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( In_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, Out_Sig, resamp_rate );
  CHANGE_RATE( Symb_Clock_In, Out_Sig, resamp_rate );

  //------------------
  //  compute decision boundaries

  Num_Symb_Rows = 1;
  for(int i=1; i<=Bits_Per_Symb/2; i++)
    Num_Symb_Rows *=2;

  I_Boundary = new double[Num_Symb_Rows-1];
  Q_Boundary = new double[Num_Symb_Rows-1];

  for(int isymb=0; isymb<Num_Symb_Rows; isymb++)
    {
    I_Boundary[isymb] = double(Samps_Per_Symb*(2 - Num_Symb_Rows + 2 * isymb));
    Q_Boundary[isymb] = double(Samps_Per_Symb*(2 - Num_Symb_Rows + 2 * isymb));
    }

}
//==============================================
QamOptimalDemod::~QamOptimalDemod( void ){ };
//==============================================

void QamOptimalDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = Out_Sig->GetSampIntvl();

  I_Integ_Val = 0.0;
  Q_Integ_Val = 0.0;

}

//============================================
int QamOptimalDemod::Execute()
{
  byte_t *out_sig_ptr;
  byte_t i_decis, q_decis;
  std::complex<float> *in_sig_ptr;
  bit_t *symb_clock_in_ptr;
  std::complex<float> in_val;
  double i_integ_val, q_integ_val;
  double max_val=0.0;
  int is;
  byte_t isymb, symb_decis;
  #ifdef _DEBUG
    *DebugFile << "In QamOptimalDemod::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  i_integ_val = I_Integ_Val;
  q_integ_val = Q_Integ_Val;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;

    I_Integ_Val += std::real(in_val);
    Q_Integ_Val += std::imag(in_val);

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      i_decis = 0;
      for(isymb=0; isymb<Num_Symb_Rows-1; isymb++)
        {
        if(I_Integ_Val < I_Boundary[isymb]) break;
        i_decis++;
        }

      q_decis = 0;
      for(isymb=0; isymb<Num_Symb_Rows-1; isymb++)
        {
        if(Q_Integ_Val < Q_Boundary[isymb]) break;
        q_decis++;
        }

      symb_decis = q_decis + Num_Symb_Rows * i_decis;
      *out_sig_ptr = symb_decis;
      out_sig_ptr++;

      I_Integ_Val = 0.0;
      Q_Integ_Val = 0.0;
      }
    symb_clock_in_ptr++;
    }
  //Integ_Val = integ_val;
  return(_MES_AOK);
}

