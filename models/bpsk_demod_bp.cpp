//
//  File = bpsk_demod_bp.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bpsk_demod_bp.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

BpskBandpassDemod::BpskBandpassDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< float >* in_sig,
                                  Signal< float >* ref_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< bit_t >* decis_out )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(BpskBandpassDemod);

  ENABLE_MULTIRATE;

  //-----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Samps_Per_Symb);
  GET_DOUBLE_PARM(Dly_To_Start);

  //--------------------------------------
  //  Connect input and output signals

  Decis_Out = decis_out;
  Symb_Clock_In = symb_clock_in;
  In_Sig = in_sig;
  Ref_Sig = ref_sig;

  MAKE_OUTPUT( Decis_Out );
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( In_Sig );
  MAKE_INPUT( Ref_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Ref_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Symb_Clock_In, Decis_Out, resamp_rate );

}
//==============================================
BpskBandpassDemod::~BpskBandpassDemod( void ){ };
//==============================================

void BpskBandpassDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = Decis_Out->GetSampIntvl();

  Integ_Val = 0.0;
}

//============================================
int BpskBandpassDemod::Execute()
{
  bit_t *decis_out_ptr;
  float *in_sig_ptr;
  float *ref_sig_ptr;
  bit_t *symb_clock_in_ptr;
  float in_val, ref_val;
  double integ_val;
  double max_val=0.0;
  int is;
  #ifdef _DEBUG
    *DebugFile << "In BpskBandpassDemod::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  ref_sig_ptr = GET_INPUT_PTR( Ref_Sig );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  decis_out_ptr = GET_OUTPUT_PTR( Decis_Out );

  integ_val = Integ_Val;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;
    ref_val = *ref_sig_ptr++;

    // correlate input signal against recovered carrier

    integ_val += in_val * ref_val;

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      if(integ_val > 0.0 )
        {
        *decis_out_ptr++ = 1;
        }
      else
        {
        *decis_out_ptr++ = 0;
        }

      integ_val = 0.0;
      }
    symb_clock_in_ptr++;
    }
  Integ_Val = integ_val;
  return(_MES_AOK);
}

