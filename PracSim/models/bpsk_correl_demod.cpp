//
//  File = bpsk_correl_demod.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bpsk_correl_demod.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

BpskCorrelationDemod::BpskCorrelationDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< std::complex< float > >* in_sig,
                                  Signal< std::complex<float> >* phase_ref_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< bit_t >* decis_out )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(BpskCorrelationDemod);

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
  Phase_Ref_Sig = phase_ref_sig;

  MAKE_OUTPUT( Decis_Out );
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( Phase_Ref_Sig );
  MAKE_INPUT( In_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Phase_Ref_Sig, Decis_Out, resamp_rate );
  CHANGE_RATE( Symb_Clock_In, Decis_Out, resamp_rate );

}
//==============================================
BpskCorrelationDemod::~BpskCorrelationDemod( void ){ };
//==============================================

void BpskCorrelationDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = Decis_Out->GetSampIntvl();

  Integ_Val = 0.0;

}

//============================================
int BpskCorrelationDemod::Execute()
{
  bit_t *decis_out_ptr;
  std::complex<float> *in_sig_ptr;
  bit_t *symb_clock_in_ptr;
  std::complex<float> in_val;
  std::complex<float> *phase_ref_sig_ptr;
  std::complex<float> phase_ref_val;
  double integ_val;
  double max_val=0.0;
  int is;
  #ifdef _DEBUG
    *DebugFile << "In BpskCorrelationDemod::Execute\0" << endl;
  #endif

  int valid_block_size = In_Sig->GetValidBlockSize();
  int out_block_size = valid_block_size/Samps_Per_Symb;
  if(valid_block_size != Samps_Per_Symb*out_block_size) {
      //error
      }
      else {
         Decis_Out->SetValidBlockSize(out_block_size);
      }

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  phase_ref_sig_ptr = GET_INPUT_PTR( Phase_Ref_Sig );
  decis_out_ptr = GET_OUTPUT_PTR( Decis_Out );

  integ_val = Integ_Val;

  for (is=0; is<valid_block_size; is++)
    {
    in_val = *in_sig_ptr++;

    // correlate input signal against recovered carrier
    phase_ref_val = *phase_ref_sig_ptr++;
//    Integ_Val += std::real(in_val * 
//                  std::complex<float>( cos(phase_ref_val), -sin(phase_ref_val) ));
    Integ_Val += std::real(in_val * phase_ref_val);

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      if(Integ_Val > 0.0 )
        {
        *decis_out_ptr++ = 1;
        }
      else
        {
        *decis_out_ptr++ = 0;
        }

      Integ_Val = 0.0;
      }
    symb_clock_in_ptr++;
    }
  //Integ_Val = integ_val;
  return(_MES_AOK);
}

