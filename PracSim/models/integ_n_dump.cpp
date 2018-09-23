//
//  File = integ_n_dump.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "integ_n_dump.h"
#include "uni_rand.h"
#include "model_graph.h"
#include "misdefs.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

IntegrateAndDump::IntegrateAndDump( char* instance_name,
                      PracSimModel* outer_model,
                      Signal<float>* in_sig,
                      Signal< bit_t >* symb_clock_in,
                      Signal<float>* samp_wave_out )
                :PracSimModel( instance_name,
                               outer_model )
{
  MODEL_NAME(IntegrateAndDump);
  In_Sig = in_sig;
  Samp_Wave_Out = samp_wave_out;
  Symb_Clock_In = symb_clock_in;

  OPEN_PARM_BLOCK;
  GET_DOUBLE_PARM( Symb_Width );

  ENABLE_MULTIRATE;
  MAKE_OUTPUT(Samp_Wave_Out);
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( In_Sig );

  // one sample per symbol at output
//  CHANGE_RATE(In_Sig, Samp_Wave_Out);

}
IntegrateAndDump::~IntegrateAndDump( void ){ };

void IntegrateAndDump::Initialize(void)
{
  In_Block_Size = In_Sig->GetBlockSize();
  In_Samp_Intvl = In_Sig->GetSampIntvl();

  Integ_Val = 0.0;
  Integ_Gain = 0.0;
};

//============================================
int IntegrateAndDump::Execute()
{
  float *in_sig_ptr, in_val;
  float *samp_wave_out_ptr;
  bit_t *symb_clock_in_ptr;
  int is, block_size;
  int out_block_size;

  double integ_val = Integ_Val;
  double integ_gain = Integ_Gain;

  samp_wave_out_ptr = GET_OUTPUT_PTR( Samp_Wave_Out );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  block_size = In_Sig->GetValidBlockSize();
  out_block_size = 0;

  for (is=0; is<block_size; is++)
    {
    in_val = *in_sig_ptr++;
    integ_val += in_val;
    integ_gain++;

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      *samp_wave_out_ptr++ = integ_val/integ_gain;
      out_block_size++;
      integ_val = 0.0;
      integ_gain = 0.0;
      }
//    else
//      {
      //*samp_wave_out_ptr++ = 0.0;
//      *samp_wave_out_ptr++ = integ_val;
//      }
    symb_clock_in_ptr++;
    }
 Samp_Wave_Out->SetValidBlockSize(out_block_size);
  Integ_Val = integ_val;
  return(_MES_AOK);
}

