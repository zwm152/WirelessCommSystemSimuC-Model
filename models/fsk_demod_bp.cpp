//
//  File = fsk_demod_bp.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "fsk_demod_bp.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

FskBandpassDemod::FskBandpassDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< float >* in_sig,
                                  Signal< bit_t >* symb_clock_in,
                                  Signal< bit_t >* decis_out,
                                  Signal< float >* lo_filt_output,
                                  Signal< float >* hi_filt_output,
                                  Signal< float >* lo_integ_sig,
                                  Signal< float >* hi_integ_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(FskBandpassDemod);

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
  Lo_Filt_Output = lo_filt_output;
  Hi_Filt_Output = hi_filt_output;
  Lo_Integ_Sig = lo_integ_sig;
  Hi_Integ_Sig = hi_integ_sig;

  MAKE_OUTPUT( Decis_Out );
  MAKE_OUTPUT( Lo_Filt_Output );
  MAKE_OUTPUT( Hi_Filt_Output );
  MAKE_OUTPUT( Lo_Integ_Sig );
  MAKE_OUTPUT( Hi_Integ_Sig );
  MAKE_INPUT( Symb_Clock_In );
  MAKE_INPUT( In_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, Decis_Out, resamp_rate );
  SAME_RATE( In_Sig, Lo_Filt_Output );
  SAME_RATE( In_Sig, Hi_Filt_Output );
  SAME_RATE( In_Sig, Lo_Integ_Sig );
  SAME_RATE( In_Sig, Hi_Integ_Sig );
  CHANGE_RATE( Symb_Clock_In, Decis_Out, resamp_rate );

  //-----------------------------------------
  //  subordinate models

  char sub_name[50];
  strcpy(sub_name, GetInstanceName());
  strcat(sub_name, ":Lo_Freq_Filter\0");
  Lo_Freq_Filter = 
        new ButterworthFilterByIir<float>(  sub_name,
                                            this,
                                            In_Sig,
                                            Lo_Filt_Output );

  strcpy(sub_name, GetInstanceName());
  strcat(sub_name, ":Hi_Freq_Filter\0");
  Hi_Freq_Filter = 
        new ButterworthFilterByIir<float>(  sub_name,
                                            this,
                                            In_Sig,
                                            Hi_Filt_Output );

  strcpy(sub_name, GetInstanceName());
  strcat(sub_name, ":Lo_Envel_Filter\0");
  Lo_Envel_Filter = 
        new ButterworthFilterByIir<float>(  sub_name,
                                            this,
                                            Lo_Filt_Output,
                                            Lo_Integ_Sig );

  strcpy(sub_name, GetInstanceName());
  strcat(sub_name, ":Hi_Envel_Filter\0");
  Hi_Envel_Filter = 
        new ButterworthFilterByIir<float>(  sub_name,
                                            this,
                                            Hi_Filt_Output,
                                            Hi_Integ_Sig );

}
//==============================================
FskBandpassDemod::~FskBandpassDemod( void ){ };
//==============================================

void FskBandpassDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Out_Samp_Intvl = Decis_Out->GetSampIntvl();
  In_Samp_Intvl = In_Sig->GetSampIntvl();

  Lo_Integ_Val = 0.0;
  Hi_Integ_Val = 0.0;
  Lo_Freq_Filter->Initialize(Block_Size, In_Samp_Intvl);

  Hi_Freq_Filter->Initialize(Block_Size, In_Samp_Intvl);

  Lo_Envel_Filter->Initialize(Block_Size, In_Samp_Intvl);

  Hi_Envel_Filter->Initialize(Block_Size, In_Samp_Intvl);

}

//============================================
int FskBandpassDemod::Execute()
{
  bit_t *decis_out_ptr;
  float *in_sig_ptr;
  float *lo_filt_output_ptr;
  float *hi_filt_output_ptr;
  float *lo_integ_sig_ptr;
  float *hi_integ_sig_ptr;
  bit_t *symb_clock_in_ptr;
  float in_val;
  double lo_integ_val, hi_integ_val;
  double max_val=0.0;
  int is;
  #ifdef _DEBUG
    *DebugFile << "In FskBandpassDemod::Execute\0" << endl;
  #endif

  /*  Run block of input samples thru low freq BP filter */
  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  lo_filt_output_ptr = GET_OUTPUT_PTR( Lo_Filt_Output );
  Lo_Freq_Filter->Execute();

  /*  Run block of input samples thru high freq BP filter */
  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  hi_filt_output_ptr = GET_OUTPUT_PTR( Hi_Filt_Output );
  Hi_Freq_Filter->Execute();

  symb_clock_in_ptr = GET_INPUT_PTR( Symb_Clock_In );
  decis_out_ptr = GET_OUTPUT_PTR( Decis_Out );

  lo_integ_sig_ptr = GET_OUTPUT_PTR( Lo_Integ_Sig );
  hi_integ_sig_ptr = GET_OUTPUT_PTR( Hi_Integ_Sig );

  lo_integ_val = Lo_Integ_Val;
  hi_integ_val = Hi_Integ_Val;

  for (is=0; is<Block_Size; is++)
    {
    *lo_filt_output_ptr = fabs(*lo_filt_output_ptr);
    lo_filt_output_ptr++;
    *hi_filt_output_ptr = fabs(*hi_filt_output_ptr);
    hi_filt_output_ptr++;
    }
  lo_filt_output_ptr = GET_OUTPUT_PTR( Lo_Filt_Output );
  hi_filt_output_ptr = GET_OUTPUT_PTR( Hi_Filt_Output );

  /*  Run block of input samples thru low freq envelope filter */
  ///in_sig_ptr = GET_INPUT_PTR( In_Sig );
  //lo_filt_output_ptr = GET_OUTPUT_PTR( Lo_Filt_Output );
  Lo_Envel_Filter->Execute();
  Hi_Envel_Filter->Execute();


  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;

    // correlate input signal against recovered carrier

    lo_integ_val += fabs(*lo_filt_output_ptr++);
    hi_integ_val += fabs(*hi_filt_output_ptr++);

    lo_integ_sig_ptr++;
    hi_integ_sig_ptr++;

    if(*symb_clock_in_ptr != 0)
      {
      // time to make a decision

      if(*hi_integ_sig_ptr > *lo_integ_sig_ptr )
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

