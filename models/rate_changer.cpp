//
//  File = rate_changer.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "rate_changer.h"
#include "model_graph.h"
#include "sinc.h"
//#include <iomanip>
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern int PassNumber;
ofstream SincFile("sinc_nnn.txt", ios::out);

//======================================================

RateChanger::RateChanger( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* in_sig,
                        Signal<float>* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(RateChanger);

  //---------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;
  GET_DOUBLE_PARM( Rate_Change_Factor );
  GET_INT_PARM( Num_Sidelobes );

  //--------------------------------------
  //  Connect input and output signals

  In_Sig = in_sig;
  Out_Sig = out_sig;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  }

//======================================
RateChanger::~RateChanger( void ){ };

//=======================================
void RateChanger::Initialize(void)
{

  //------------------
  Block_Size = Out_Sig->GetBlockSize();

  Num_Save_Samps = 2*Num_Sidelobes-1;
  Save_Buffer = new float[Num_Save_Samps];
  for(int i=0; i<Num_Save_Samps; i++)
    {
    Save_Buffer[i] = 0.0;
    }
  Cumul_Samp_Count = 0;
}
//=======================================================
int RateChanger::Execute()
{
  float *out_sig_ptr;
  float *in_sig_ptr;
  double work;
  double sum;
  double sampling_offset;
  int is, in_idx, sav_idx;

  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );
  in_sig_ptr = GET_INPUT_PTR( In_Sig );

  //sampling_offset = (PassNumber-1)*Block_Size*(Rate_Change_Factor-1.0);
  sampling_offset = PassNumber*Block_Size*(Rate_Change_Factor-1.0);

  //-------------------------------------------------------
  //  compute outputs that use inputs from previous block
  *DebugFile << "Previous Block" << endl;
  int neg_terms, pos_terms;
  double first_sinc;
  double term;
  int is_rev;
  #define SAMP_TO_DUMP 609
  #define PASS_TO_DUMP 2

  for (is=0; is<Num_Save_Samps; is++)
  //for (is=Num_Save_Samps-1; is>=0; is--)
    {
    is_rev = Num_Save_Samps - is - 1;
    sum = 0.0;
    neg_terms = 0;
    pos_terms = 0;
    //work = double(is)- Delay_In_Samps;
    //work = sampling_offset + is * Rate_Change_Factor;
    work = Num_Sidelobes + sampling_offset + is_rev * Rate_Change_Factor;
    first_sinc = work;
    //work -= Cumul_Samp_Count;
    //Cumul_Samp_Count++;

    if(is==SAMP_TO_DUMP) *DebugFile << "First Loop" << endl;
    //if( (is==SAMP_TO_DUMP) && (PassNumber==1) )
    //          SincFile << "First Loop" << endl;
    for(sav_idx=is_rev; sav_idx<Num_Save_Samps; sav_idx++)
      {
      term = Save_Buffer[sav_idx] * sinc(work-sav_idx);
      sum += term;
      if( (is==SAMP_TO_DUMP) && (PassNumber==PASS_TO_DUMP) )
                 SincFile << sav_idx << ", " << setprecision(12) 
                          << double(work-sav_idx) << ", "
                          << sinc(work-sav_idx) << ", "
                          << Save_Buffer[sav_idx] << ", "
                          << term << endl;
      if( (work-sav_idx) < 0.0) neg_terms++;
      if( (work-sav_idx) > 0.0) pos_terms++;
      }
    work -= Num_Save_Samps;
    if(is==SAMP_TO_DUMP) *DebugFile << "Second Loop" << endl;
    //if( (is==SAMP_TO_DUMP) && (PassNumber==1) )
    //          SincFile << "Second Loop" << endl;

    for(in_idx=0; in_idx<=is_rev; in_idx++)
      {
      term = in_sig_ptr[in_idx] * sinc(work-in_idx);
      sum += term;

      if( (is==SAMP_TO_DUMP) && (PassNumber==PASS_TO_DUMP) )
                 SincFile << in_idx << ", " << setprecision(12) 
                          << double(work-in_idx) << ", "
                          << sinc(work-in_idx) << ", "
                          << in_sig_ptr[in_idx] << ", "
                          << term << endl;
      if( (work-in_idx) < 0.0) neg_terms++;
      if( (work-in_idx) > 0.0) pos_terms++;
      }
    *DebugFile << is << " sum = " << sum << ", " << pos_terms << ", " 
              << neg_terms << ", " << first_sinc << endl;
    *out_sig_ptr++ = sum;
    //if(is==2) exit(0);
    }

  //----------------------------------------------------------
  //  compute outputs that use only inputs from current block
  *DebugFile << "Current Block" << endl;
  //if( (is==SAMP_TO_DUMP) && (PassNumber==1) )
  //          SincFile << "Current Block" << endl;

  for (is=Num_Sidelobes-1; is<Block_Size-Num_Sidelobes; is++)
    {
    sum = 0.0;
    neg_terms = 0;
    pos_terms = 0;
    //work = double(is)- Delay_In_Samps;
    work = sampling_offset + is * Rate_Change_Factor;
    ///work = 1 + sampling_offset + is * Rate_Change_Factor;
    //work -= Cumul_Samp_Count;
    //Cumul_Samp_Count++;

    first_sinc = work - (is - Num_Sidelobes + 1);
    //for(  in_idx = is - Num_Sidelobes + 1;
    for(  in_idx = is - Num_Sidelobes + 1;
          in_idx <= is + Num_Sidelobes;
          in_idx++)
      {
      term = in_sig_ptr[in_idx] * sinc(work-in_idx);
      sum += term;
      if( (is==((Num_Sidelobes-1)+0)) && (PassNumber==PASS_TO_DUMP) )
                SincFile << in_idx << ", " << setprecision(12) 
                          << double(work-in_idx) << ", "
                          << sinc(work-in_idx) << ", "
                          << in_sig_ptr[in_idx] << ", "
                          << term << endl;
      if( (work-in_idx) < 0.0) neg_terms++;
      if( (work-in_idx) > 0.0) pos_terms++;
      }
    *DebugFile << is << "sum = " << sum << ", " << pos_terms << ", "
              << neg_terms << ", " << first_sinc << endl;
    *out_sig_ptr++ = sum;
//    *out_sig_ptr++ = 0.0;
    }

  //---------------------------------------------------------
  // save the input samples needed for next pass

  for(sav_idx=0; sav_idx<Num_Save_Samps; sav_idx++)
    {
    Save_Buffer[sav_idx] = in_sig_ptr[Block_Size-Num_Save_Samps+sav_idx];
    }
  return(_MES_AOK);
}

