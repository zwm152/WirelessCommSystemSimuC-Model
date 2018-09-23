//
//  File = m_pskharddem.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "m_pskharddem.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

M_PskHardDemod::M_PskHardDemod( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal< std::complex< float > >* in_sig,
                                  Signal< std::complex< float > >* integ_sig,
                                  Signal< byte_t >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(M_PskHardDemod);

  ENABLE_MULTIRATE;

  //-----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_DOUBLE_PARM(Phase_Unbal);
  GET_INT_PARM(Bits_Per_Symb);
  GET_INT_PARM(Samps_Per_Symb);
  GET_DOUBLE_PARM(Dly_To_Start);

  //int block_size = ParmInput.GetIntParm("block_size\0");
  //double samp_intvl = ParmInput.GetDoubleParm("samp_intvl\0");

  //--------------------------------------
  //  Connect input and output signals

  Out_Sig = out_sig;
  Integ_Sig = integ_sig;
  In_Sig = in_sig;

  MAKE_OUTPUT( Integ_Sig );
  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  double resamp_rate = 1.0/double(Samps_Per_Symb);
  CHANGE_RATE( In_Sig, Out_Sig, resamp_rate );
  SAME_RATE( In_Sig, Integ_Sig );

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
M_PskHardDemod::~M_PskHardDemod( void ){ };
//==============================================

void M_PskHardDemod::Initialize(void)
{
  Block_Size = In_Sig->GetBlockSize();
  Integ_Val = std::complex<float>(0.0,0.0);
  Out_Samp_Intvl = Out_Sig->GetSampIntvl();
}

//============================================
int M_PskHardDemod::Execute()
{
  byte_t *out_sig_ptr;
  //double ref_phase, phase_unbal;
  std::complex<float> *in_sig_ptr, *integ_sig_ptr;
  std::complex<float> in_val, integ_val;
  float angle;
  int is;
  int isymb;
  byte_t symb_decis;
  #ifdef _DEBUG
    *DebugFile << "In M_PskHardDemod::Execute\0" << endl;
  #endif

  in_sig_ptr = GET_INPUT_PTR( In_Sig );
  integ_sig_ptr = GET_INPUT_PTR( Integ_Sig );
  out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

  integ_val = Integ_Val;

  if(PassNumber == 1)
    {
    int num_filler_samps = int(ceil(Dly_To_Start/Out_Samp_Intvl));
    for(int n=0; n<num_filler_samps; n++)
      {
      *out_sig_ptr++ = 0;
      }
    }

  //phase_unbal = Phase_Unbal;

  int samps_in_curr_symb = -Samps_Per_Symb;
  for (is=0; is<Block_Size; is++)
    {
    in_val = *in_sig_ptr++;
    integ_val += in_val;
    *integ_sig_ptr++ = integ_val;

    samps_in_curr_symb++;

    if(samps_in_curr_symb >= 0)
      {
      // time to make a decision

      angle = std::arg<float>(integ_val);
      integ_val = std::complex<float>(0.0,0.0);
      if(angle < 0.0) angle += TWO_PI;

      for(isymb=0; isymb<Num_Diff_Symbs; isymb++)
        {
        symb_decis = isymb;
        if(angle < Decis_Bound[isymb]) break;
        symb_decis = 0;
        }
      *out_sig_ptr = symb_decis;
      out_sig_ptr++;
      samps_in_curr_symb -= Samps_Per_Symb;
      }
    }
  Integ_Val = integ_val;
  return(_MES_AOK);
}

