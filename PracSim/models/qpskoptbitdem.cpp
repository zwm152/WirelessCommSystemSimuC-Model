//
//  File = qpskoptbitdem.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "qpskoptbitdem.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

QpskOptimalBitDemod::QpskOptimalBitDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< std::complex< float > >* in_sig,
                                  Signal< std::complex< float > >* carrier_ref_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< bit_t >* i_decis_out,
                                  Signal< bit_t >* q_decis_out )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QpskOptimalBitDemod);

  ENABLE_MULTIRATE;

  //-----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Samps_Per_Symb);
  GET_BOOL_PARM(Constel_Offset_Enabled);

  //--------------------------------------
  //  Connect input and output signals

  I_Decis_Out = i_decis_out;
  Q_Decis_Out = q_decis_out;
  Symb_Clock_In = symb_clock_in;
  Carrier_Ref_Sig = carrier_ref_sig;
  In_Sig = in_sig;

  MAKE_OUTPUT( I_Decis_Out );
  MAKE_OUTPUT( Q_Decis_Out );
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( Carrier_Ref_Sig );
  MAKE_INPUT( In_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, I_Decis_Out, resamp_rate );
  CHANGE_RATE( In_Sig, Q_Decis_Out, resamp_rate );
  CHANGE_RATE( Carrier_Ref_Sig, I_Decis_Out, resamp_rate );
  CHANGE_RATE( Carrier_Ref_Sig, Q_Decis_Out, resamp_rate );
  CHANGE_RATE( Symb_Clock_In, I_Decis_Out, resamp_rate );

}
//==============================================
QpskOptimalBitDemod::~QpskOptimalBitDemod( void ){ };
//==============================================

void QpskOptimalBitDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = I_Decis_Out->GetSampIntvl();

  Integ_Val = new double[4];
  for( int isymb=0; isymb<4; isymb++)
    {
    Integ_Val[isymb] = 0.0;
    }
  if(Constel_Offset_Enabled)
    {
    Constel_Offset_Rot = std::complex<float>(sqrt(2.0)/2.0, sqrt(2.0)/2.0);
    }
  else
    {
    Constel_Offset_Rot = std::complex<float>(1.0, 0.0);
    }
}

//============================================
int QpskOptimalBitDemod::Execute()
{
  bit_t *i_decis_out_ptr;
  bit_t *q_decis_out_ptr;
  std::complex<float> *in_sig_ptr;
  bit_t *symb_clock_in_ptr;
  std::complex<float> in_val;
  std::complex<float> constel_offset_rot;
  std::complex<float> correl_ref;
  std::complex<float> *carrier_ref_sig_ptr;
  std::complex<float> carrier_val;
  float correl_val;
  double *integ_val;
  double max_val=0.0;
  int is;
  byte_t isymb, symb_decis;
  #ifdef _DEBUG
    *DebugFile << "In QpskOptimalBitDemod::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  carrier_ref_sig_ptr = GET_INPUT_PTR( Carrier_Ref_Sig );
  i_decis_out_ptr = GET_OUTPUT_PTR( I_Decis_Out );
  q_decis_out_ptr = GET_OUTPUT_PTR( Q_Decis_Out );

  constel_offset_rot = Constel_Offset_Rot;
  std::complex<float> cmpx_j = std::complex<float>(0.0,-1.0);
  integ_val = Integ_Val;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;
    carrier_val = *carrier_ref_sig_ptr++;

    correl_ref = std::conj(constel_offset_rot * carrier_val);

    // correlate against s0
    correl_val = std::real(in_val * correl_ref);
    integ_val[0] += correl_val;

    // correlate against s2
    integ_val[2] -= correl_val;

    // correlate against s1
    correl_ref *= cmpx_j;
    correl_val = std::real(in_val * correl_ref);
    integ_val[1] += correl_val;

    // correlate against s3
    integ_val[3] -= correl_val;

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      max_val = Integ_Val[0];
      symb_decis = 0;
      for(isymb=1; isymb<4; isymb++)
        {
        if(integ_val[isymb] > max_val)
          {
          max_val = integ_val[isymb];
          symb_decis = isymb;
          }
        }
      switch (symb_decis)
        {
        case 0:
          *i_decis_out_ptr++ = 1;
          *q_decis_out_ptr++ = 1;
          break;
        case 1:
          *i_decis_out_ptr++ = 0;
          *q_decis_out_ptr++ = 1;
          break;
        case 2:
          *i_decis_out_ptr++ = 0;
          *q_decis_out_ptr++ = 0;
          break;
        case 3:
          *i_decis_out_ptr++ = 1;
          *q_decis_out_ptr++ = 0;
          break;
        }

      for(isymb=0; isymb<4; isymb++)
        {
        integ_val[isymb] = 0.0;
        }
      }
    symb_clock_in_ptr++;
    }
  return(_MES_AOK);
}

