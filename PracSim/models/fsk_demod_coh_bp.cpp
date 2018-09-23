//
//  File = fsk_demod_coh_bp.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "fsk_demod_coh_bp.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

FskCoherentBandpassDemod::FskCoherentBandpassDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< float >* in_sig,
                                  Signal< float >* lo_ref_sig,
                                  Signal< float >* hi_ref_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< bit_t >* decis_out,
                                  Signal< float >* lo_integ_sig,
                                  Signal< float >* hi_integ_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(FskCoherentBandpassDemod);

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
  Lo_Ref_Sig = lo_ref_sig;
  Hi_Ref_Sig = hi_ref_sig;
  Lo_Integ_Sig = lo_integ_sig;
  Hi_Integ_Sig = hi_integ_sig;

  MAKE_OUTPUT( Decis_Out );
  MAKE_OUTPUT( Lo_Integ_Sig );
  MAKE_OUTPUT( Hi_Integ_Sig );
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( In_Sig );
  MAKE_INPUT( Lo_Ref_Sig );
  MAKE_INPUT( Hi_Ref_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Lo_Ref_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Hi_Ref_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Symb_Clock_In, Decis_Out, resamp_rate );
  SAME_RATE( Lo_Ref_Sig, Lo_Integ_Sig );
  SAME_RATE( Hi_Ref_Sig, Hi_Integ_Sig );

}
//==============================================
FskCoherentBandpassDemod::~FskCoherentBandpassDemod( void ){ };
//==============================================

void FskCoherentBandpassDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = Decis_Out->GetSampIntvl();

  Lo_Integ_Val = 0.0;
  Hi_Integ_Val = 0.0;
}

//============================================
int FskCoherentBandpassDemod::Execute()
{
  bit_t *decis_out_ptr;
  float *in_sig_ptr;
  float *lo_ref_sig_ptr;
  float *hi_ref_sig_ptr;
  float *lo_integ_sig_ptr;
  float *hi_integ_sig_ptr;
  bit_t *symb_clock_in_ptr;
  float in_val;
  double lo_integ_val, hi_integ_val;
  double max_val=0.0;
  int is;
  #ifdef _DEBUG
    *DebugFile << "In FskCoherentBandpassDemod::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  lo_ref_sig_ptr = GET_INPUT_PTR( Lo_Ref_Sig );
  hi_ref_sig_ptr = GET_INPUT_PTR( Hi_Ref_Sig );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  decis_out_ptr = GET_OUTPUT_PTR( Decis_Out );
  lo_integ_sig_ptr = GET_OUTPUT_PTR( Lo_Integ_Sig );
  hi_integ_sig_ptr = GET_OUTPUT_PTR( Hi_Integ_Sig );

  lo_integ_val = Lo_Integ_Val;
  hi_integ_val = Hi_Integ_Val;

  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;

    // correlate input signal against recovered carrier

    lo_integ_val += in_val * (*lo_ref_sig_ptr++);
    hi_integ_val += in_val * (*hi_ref_sig_ptr++);
    *lo_integ_sig_ptr++ = float(lo_integ_val/float(Samps_Per_Symb));
    *hi_integ_sig_ptr++ = float(hi_integ_val/float(Samps_Per_Symb));

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      if(hi_integ_val > lo_integ_val )
        {
        *decis_out_ptr++ = 1;
        }
      else
        {
        *decis_out_ptr++ = 0;
        }

      lo_integ_val = 0.0;
      hi_integ_val = 0.0;
      }
    symb_clock_in_ptr++;
    }
  Lo_Integ_Val = lo_integ_val;
  Hi_Integ_Val = hi_integ_val;
  return(_MES_AOK);
}

