//
//  File = integ_dump_slice.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "integ_dump_slice.h"
#include "uni_rand.h"
#include "model_graph.h"
#include "misdefs.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

IntegrateDumpAndSlice::IntegrateDumpAndSlice( char* instance_name,
                      PracSimModel* outer_model,
                      Signal<float>* in_sig,
                      //Signal<float>* corr_sig,
                      Signal< bit_t >* symb_clock_in,
                      Signal<float>* samp_wave_out,
                      Signal<bit_t>* out_sig )
                :PracSimModel( instance_name,
                               outer_model )
{
  MODEL_NAME(IntegrateDumpAndSlice);
  In_Sig = in_sig;
  //Corr_Sig = corr_sig;
  Out_Sig = out_sig;
  Samp_Wave_Out = samp_wave_out;
  Symb_Clock_In = symb_clock_in;

  OPEN_PARM_BLOCK;
  GET_DOUBLE_PARM( Symb_Width );

  ENABLE_MULTIRATE;
  MAKE_OUTPUT( Out_Sig );
  MAKE_OUTPUT(Samp_Wave_Out);
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( In_Sig );
  //MAKE_INPUT( Corr_Sig );

  // one sample per symbol at output
  SET_SAMP_INTVL( Out_Sig, Symb_Width );
  SAME_RATE(In_Sig, Samp_Wave_Out);
  //SAME_RATE(Corr_Sig, Samp_Wave_Out);

}
IntegrateDumpAndSlice::~IntegrateDumpAndSlice( void ){ };

void IntegrateDumpAndSlice::Initialize(void)
{
  Out_Block_Size = Out_Sig->GetBlockSize();
  In_Block_Size = In_Sig->GetBlockSize();
  In_Samp_Intvl = In_Sig->GetSampIntvl();
  Out_Samp_Intvl = Out_Sig->GetSampIntvl();

  Integ_Val = 0.0;
};

//============================================
int IntegrateDumpAndSlice::Execute()
{
  float *in_sig_ptr, in_val;
  //float *corr_sig_ptr, corr_val;
  float *samp_wave_out_ptr;
  bit_t *out_sig_ptr, symb_decis;
  bit_t *symb_clock_in_ptr;
  int is;

  double integ_val = Integ_Val;

   Out_Sig->SetValidBlockSize(Out_Block_Size);
   Samp_Wave_Out->SetValidBlockSize(In_Block_Size);

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
  samp_wave_out_ptr = GET_OUTPUT_PTR( Samp_Wave_Out );
  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  //corr_sig_ptr = GET_INPUT_PTR( Corr_Sig );

  for (is=0; is<In_Block_Size; is++)
    {
    in_val = *in_sig_ptr++;
    //corr_val = *corr_sig_ptr++;

    //integ_val += in_val*corr_val;
    integ_val += in_val;

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      if(integ_val < 0.0) symb_decis = 0;
      else symb_decis = 1;
      *out_sig_ptr++ = symb_decis;
      *samp_wave_out_ptr++ = integ_val;
      integ_val = 0.0;
      }
    else
      {
      //*samp_wave_out_ptr++ = 0.0;
      *samp_wave_out_ptr++ = integ_val;
      }
    symb_clock_in_ptr++;
    }
  Integ_Val = integ_val;
  return(_MES_AOK);
}

