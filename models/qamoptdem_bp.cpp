//
//  File = qamoptdem_bp.cpp
//

#include <stdlib.h>
#include <fstream.h>
#include "parmfile.h"
#include "qamoptdem_bp.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

QamOptimalBandpassDemod::QamOptimalBandpassDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< float >* in_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< byte_t >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(QamOptimalBandpassDemod);

  ENABLE_MULTIRATE;

  //-----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Phase_Unbal);
  GET_DOUBLE_PARM(Carrier_Freq);
  GET_INT_PARM(Bits_Per_Symb);
  GET_INT_PARM(Samps_Per_Symb);
  GET_DOUBLE_PARM(Dly_To_Start);

  Carrier_Freq_Rad = TWO_PI * Carrier_Freq;

  //int block_size = ParmInput.GetIntParm("block_size\0");
  //double samp_intvl = ParmInput.GetDoubleParm("samp_intvl\0");

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

  Num_Diff_Symbs = 1;
  for(int i=1; i<=Bits_Per_Symb; i++)
    Num_Diff_Symbs *=2;

  double offset = TWO_PI/(2.0*Num_Diff_Symbs);
  Decis_Bound = new float[Num_Diff_Symbs];
  for(int isymb=0; isymb<Num_Diff_Symbs; isymb++)
    {
    Decis_Bound[isymb] = float(offset + isymb*TWO_PI/double(Num_Diff_Symbs));
    }
}
//==============================================
QamOptimalBandpassDemod::~QamOptimalBandpassDemod( void ){ };
//==============================================

void QamOptimalBandpassDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
//  Out_Samp_Intvl = Out_Sig->GetSampIntvl();
  //
  //  set up table of phase references
  Ref_Angle = new float[Num_Diff_Symbs];
  Integ_Val = new double[Num_Diff_Symbs];

  for( int isymb=0; isymb<Num_Diff_Symbs; isymb++)
    {
    Integ_Val[isymb] = 0.0;
    Ref_Angle[isymb] = TWO_PI * isymb/double(Num_Diff_Symbs);
    }
  Time = 0.0;
  Samp_Intvl = In_Sig->GetSampIntvl();
}

//============================================
int QamOptimalBandpassDemod::Execute()
{
  byte_t *out_sig_ptr;
  float *in_sig_ptr;
  bit_t *symb_clock_in_ptr;
  float in_val;
  double *integ_val;
  double max_val=0.0;
  double omega, time, time_base;
  double samp_intvl;
  int is;
  byte_t isymb, symb_decis;
  #ifdef _DEBUG
    *DebugFile << "In QamOptimalBandpassDemod::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  omega = Carrier_Freq_Rad;
  time_base = Time;
  samp_intvl = Samp_Intvl;
  integ_val = Integ_Val;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;

    time = time_base + is * samp_intvl;
    // correlate input signal against all possible reference phases
    for( isymb=0; isymb<Num_Diff_Symbs; isymb++)
      {
      Integ_Val[isymb] += in_val * cos(omega*time + Ref_Angle[isymb]);
      }

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      max_val = Integ_Val[0];
      symb_decis = 0;
      for(isymb=1; isymb<Num_Diff_Symbs; isymb++)
        {
        if(Integ_Val[isymb] > max_val)
          {
          max_val = Integ_Val[isymb];
          symb_decis = isymb;
          }
        }
      *out_sig_ptr = symb_decis;
      out_sig_ptr++;

      for(isymb=0; isymb<Num_Diff_Symbs; isymb++)
        {
        Integ_Val[isymb] = 0.0;
        }
      }
    symb_clock_in_ptr++;
    }
  Time = time_base + samp_intvl * Block_Size;
  //Integ_Val = integ_val;
  return(_MES_AOK);
}

