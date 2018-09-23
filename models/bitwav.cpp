//
//  File = bitwav.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "bitwav.h"
#include "model_graph.h"
//#include "uni_rand.h"
extern ParmFile *ParmInput;

//======================================================

BitsToWave::BitsToWave( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<bit_t>* in_sig,
                        Signal<float>* out_sig,
                        Signal<bit_t>* bit_clock_out )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(BitsToWave);

  ENABLE_MULTIRATE;

  //---------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;
  GET_DOUBLE_PARM( Pulse_Duration );
  GET_DOUBLE_PARM( Delay_To_First_Edge );
  GET_DOUBLE_PARM( Lo_Val );
  GET_DOUBLE_PARM( Hi_Val );
  GET_FLOAT_PARM( Samps_Per_Bit );

  //--------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;
  Bit_Clock_Out = bit_clock_out;

  MAKE_OUTPUT( Out_Sig );
  MAKE_OUTPUT( Bit_Clock_Out );
  MAKE_INPUT( In_Sig );

  double resamp_rate = double(Samps_Per_Bit);

  // one sample per bit at input
  SET_SAMP_INTVL( In_Sig, Pulse_Duration)
  CHANGE_RATE( In_Sig, Out_Sig, resamp_rate );
  CHANGE_RATE( In_Sig, Bit_Clock_Out, resamp_rate );
  }

//======================================
BitsToWave::~BitsToWave( void ){ };

//=======================================
void BitsToWave::Initialize(void)
{

  //------------------
  Last_Bit_Val = 0.0;

  double samp_intvl = Out_Sig->GetSampIntvl();
  Samps_In_Curr_Bit = (Pulse_Duration - Delay_To_First_Edge)
                      /samp_intvl - 1.0;

  Out_Avg_Block_Size = Out_Sig->GetBlockSize();
  In_Avg_Block_Size = In_Sig->GetBlockSize();
  Samps_Per_Bit = float(Out_Avg_Block_Size)/In_Avg_Block_Size;

}
//=======================================================
int BitsToWave::Execute()
{
  float last_bit_val;
  double samps_in_curr_bit, samps_per_bit;
  float lo_val, hi_val;
  int next_bit_val;
  float *waveform_out;
  bit_t *bits_in;
  bit_t *bit_clock_out_ptr;
  int is;
  int in_block_size;
  int out_block_size;

  last_bit_val = Last_Bit_Val;
  samps_in_curr_bit = Samps_In_Curr_Bit;
  samps_per_bit = Samps_Per_Bit;
  lo_val = Lo_Val;
  hi_val = Hi_Val;
  waveform_out = GET_OUTPUT_PTR( Out_Sig );
  bit_clock_out_ptr = GET_OUTPUT_PTR( Bit_Clock_Out );
  bits_in = GET_INPUT_PTR( In_Sig );
  in_block_size = In_Sig->GetValidBlockSize();
  out_block_size = int(samps_per_bit * in_block_size);
  Out_Sig->SetValidBlockSize(out_block_size);
  Bit_Clock_Out->SetValidBlockSize(out_block_size);

  for (is=0; is<out_block_size; is++)
    {
    samps_in_curr_bit++;
    if(samps_in_curr_bit >= samps_per_bit)
      {
      next_bit_val = *bits_in++;
      if(next_bit_val == 0)
        last_bit_val = lo_val;
      else
        last_bit_val = hi_val;

      samps_in_curr_bit -= samps_per_bit;
      }
    *waveform_out++ = last_bit_val;
    //*waveform_out++ = is;

    if(samps_in_curr_bit+1 >= samps_per_bit)
      {
      *bit_clock_out_ptr++ = 1;
      }
    else
      {
      *bit_clock_out_ptr++ = 0;
      }
    }
  Last_Bit_Val = last_bit_val;
  Samps_In_Curr_Bit = samps_in_curr_bit;
  return(_MES_AOK);
}

