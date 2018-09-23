//
//  File = qam_decoder.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "qam_decoder.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern int PassNumber;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================

QamSymbolDecoder::QamSymbolDecoder( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal<float>* i_in_sig,
                                  Signal<float>* q_in_sig,
                                  Signal< byte_t >* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
   MODEL_NAME(QamSymbolDecoder);

   ENABLE_MULTIRATE;

   //-----------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;

   GET_INT_PARM(Bits_Per_Symb);

   //--------------------------------------
   //  Connect input and output signals

   Out_Sig = out_sig;
   I_In_Sig = i_in_sig;
   Q_In_Sig = q_in_sig;

   MAKE_OUTPUT( Out_Sig );
   MAKE_INPUT( I_In_Sig );
   MAKE_INPUT( Q_In_Sig );

   SAME_RATE( I_In_Sig, Out_Sig );
   SAME_RATE( Q_In_Sig, Out_Sig );

   //------------------
   //  compute decision boundaries

   Num_Symb_Rows = 1;
   for(int i=1; i<=Bits_Per_Symb/2; i++)
    Num_Symb_Rows *=2;

   I_Boundary = new double[Num_Symb_Rows-1];
   Q_Boundary = new double[Num_Symb_Rows-1];

   for(int isymb=0; isymb<Num_Symb_Rows-1; isymb++)
      {
      I_Boundary[isymb] = double(2 - Num_Symb_Rows + 2 * isymb)/2.0;
      Q_Boundary[isymb] = double(2 - Num_Symb_Rows + 2 * isymb)/2.0;
      }

}
//==============================================
QamSymbolDecoder::~QamSymbolDecoder( void ){ };
//==============================================

void QamSymbolDecoder::Initialize(void)
{
  Block_Size = I_In_Sig->GetBlockSize();
  Out_Samp_Intvl = Out_Sig->GetSampIntvl();
}

//============================================
int QamSymbolDecoder::Execute()
{
   byte_t *out_sig_ptr;
   byte_t i_decis, q_decis;
   float *i_in_sig_ptr, *q_in_sig_ptr;
   float i_in_val, q_in_val;
   int is;
   byte_t isymb, symb_decis;
   #ifdef _DEBUG
    *DebugFile << "In QamSymbolDecoder::Execute\0" << endl;
   #endif

   i_in_sig_ptr = GET_INPUT_PTR( I_In_Sig );
   q_in_sig_ptr = GET_INPUT_PTR( Q_In_Sig );
   out_sig_ptr = GET_OUTPUT_PTR( Out_Sig );

   for (is=0; is<Block_Size; is++)
      {
      i_in_val = *i_in_sig_ptr++;
      q_in_val = *q_in_sig_ptr++;

      i_decis = 0;
      for(isymb=0; isymb<Num_Symb_Rows-1; isymb++)
      {
      if(i_in_val < I_Boundary[isymb]) break;
      i_decis++;
      }

      q_decis = 0;
      for(isymb=0; isymb<Num_Symb_Rows-1; isymb++)
      {
      if(q_in_val < Q_Boundary[isymb]) break;
      q_decis++;
      }

      symb_decis = q_decis + Num_Symb_Rows * i_decis;
      *out_sig_ptr = symb_decis;
      out_sig_ptr++;
      }
   return(_MES_AOK);
}

