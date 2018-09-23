//
//  File = mpsk_symb_to_wave.cpp
//

#include <stdlib.h>
#include "parmfile.h"
#include "mpsk_symbtowave.h"
#include "model_error.h"
#include "misdefs.h"
#include "model_graph.h"
#include "typedefs.h"
extern ParmFile *ParmInput;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

MpskSymbsToQuadWaves::MpskSymbsToQuadWaves( char* instance_name,
                              PracSimModel* outer_model,
                              Signal<byte_t>* in_symb_seq,
                              Signal<float>* i_out_sig,
                              Signal<float>* q_out_sig,
                              Signal<bit_t>* symb_clock_out )
                :PracSimModel(instance_name,
                              outer_model)
{
  MODEL_NAME(MpskSymbsToQuadWaves);

  ENABLE_MULTIRATE;

  //----------------------------------------
  //  Read model config parms

  OPEN_PARM_BLOCK;

  GET_INT_PARM(Bits_Per_Symb);
  GET_INT_PARM(Samps_Per_Symb);
  GET_DOUBLE_PARM(Symb_Duration);

  //-------------------------------------
  //  Connect input and output signals

  In_Symb_Seq = in_symb_seq;
  I_Out_Sig = i_out_sig;
  Q_Out_Sig = q_out_sig;
  Symb_Clock_Out = symb_clock_out;

  MAKE_OUTPUT( I_Out_Sig );
  MAKE_OUTPUT( Q_Out_Sig );
  MAKE_OUTPUT( Symb_Clock_Out );
  MAKE_INPUT( In_Symb_Seq );

  // one sample per symbol from input
  double resamp_rate = double(Samps_Per_Symb);

  SET_SAMP_INTVL( In_Symb_Seq, Symb_Duration);
  CHANGE_RATE( In_Symb_Seq, Symb_Clock_Out, resamp_rate );
  CHANGE_RATE( In_Symb_Seq, I_Out_Sig, resamp_rate );
  CHANGE_RATE( In_Symb_Seq, Q_Out_Sig, resamp_rate );


  //-----------------------------------------
  // Set up derived parms

  Num_Diff_Symbs = 1;
  for(int i=1; i<=Bits_Per_Symb; i++)
    Num_Diff_Symbs *=2;

  //------------------------------------------------------------
  //  generate tables of I & Q values for desired constellation

  double angle;
  double magnitude = 1.0;
  float work;
  I_Compon = new float[Num_Diff_Symbs];
  Q_Compon = new float[Num_Diff_Symbs];

  for(int isymb=0; isymb<Num_Diff_Symbs; isymb++)
    {
    angle = isymb * TWO_PI/double(Num_Diff_Symbs);
    work = float(magnitude * cos(angle));
    I_Compon[isymb] = work;
    work = float(magnitude * sin(angle));
    Q_Compon[isymb] = work;
    }

}
//===========================================
MpskSymbsToQuadWaves::~MpskSymbsToQuadWaves( void ){ };
//===========================================

void MpskSymbsToQuadWaves::Initialize(void)
{
  In_Block_Size = In_Symb_Seq->GetBlockSize();
  Out_Block_Size = I_Out_Sig->GetBlockSize();

}

//============================================
int MpskSymbsToQuadWaves::Execute(void)
{
  byte_t *in_symb_seq_ptr;
  float i_comp, q_comp;
  float *i_out_sig_ptr, *q_out_sig_ptr;
  int in_block_size;
  int out_block_size;

  bit_t *symb_clock_out_ptr;
  int is;
  #ifdef _DEBUG
    *DebugFile << "In MpskSymbToCmpxWave::Execute\0" << endl;
  #endif

  i_out_sig_ptr = GET_OUTPUT_PTR( I_Out_Sig );
  q_out_sig_ptr = GET_OUTPUT_PTR( Q_Out_Sig );
  symb_clock_out_ptr = GET_OUTPUT_PTR( Symb_Clock_Out );
  in_symb_seq_ptr = GET_INPUT_PTR( In_Symb_Seq );
  in_block_size = In_Symb_Seq->GetValidBlockSize();

  out_block_size = Samps_Per_Symb * in_block_size;
  I_Out_Sig->SetValidBlockSize(out_block_size);
  Q_Out_Sig->SetValidBlockSize(out_block_size);
  Symb_Clock_Out->SetValidBlockSize(out_block_size);

  for (is=0; is<in_block_size; is++)
    {
    //  get input symbol and split into ideal I & Q components
    i_comp = I_Compon[*in_symb_seq_ptr];
    q_comp = Q_Compon[*in_symb_seq_ptr];
    in_symb_seq_ptr++;

    for(int isamp=0; isamp<Samps_Per_Symb; isamp++)
      {
      *i_out_sig_ptr++ = i_comp;
      *q_out_sig_ptr++ = q_comp;

      if(isamp == (Samps_Per_Symb-1))
        *symb_clock_out_ptr++ = 1;
      else
        *symb_clock_out_ptr++ = 0;
      }
    }
  return(_MES_AOK);
}

