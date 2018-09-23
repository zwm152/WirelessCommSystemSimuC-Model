//
//  File = msk_genie.cpp
//

#include <stdlib.h>
//#include <fstream>
#include "parmfile.h"
#include "msk_genie.h"
#include "misdefs.h"
#include "model_graph.h"
extern ParmFile *ParmInput;

//======================================================

MskShapeGenie::MskShapeGenie( char* instance_name,
                            PracSimModel* outer_model,
                            Signal< float >* i_shape_sig,
                            Signal< float >* q_shape_sig)
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(MskShapeGenie);

  //----------------------------------------
  //  Read model config parms
  
  OPEN_PARM_BLOCK;
  
  GET_DOUBLE_PARM(Bit_Durat);
  GET_BOOL_PARM(Shaping_Is_Bipolar);
  //GET_INT_PARM(block_size);
  //GET_DOUBLE_PARM(samp_intvl);

  //-------------------------------------
  //  Connect input and output signals

  I_Shape_Sig = i_shape_sig;
  Q_Shape_Sig = q_shape_sig;

  MAKE_INPUT( I_Shape_Sig );
  MAKE_INPUT( Q_Shape_Sig );

  //--------------------------------------------
  // Set up derived parms

  Pi_Over_Bit_Dur = PI/Bit_Durat;
}
//===========================================
MskShapeGenie::~MskShapeGenie( void ){ };
//==========================================
void MskShapeGenie::Initialize(void)
{
  Samps_Out_Cnt = 0;
  Block_Size = I_Shape_Sig->GetBlockSize();
  Samp_Intvl = I_Shape_Sig->GetSampIntvl();
}
//===========================================
int MskShapeGenie::Execute(void)
{
  float *i_shape_sig_ptr, *q_shape_sig_ptr;
  int samps_out_cnt;
  double samp_intvl;
  double pi_over_bit_dur, argument;
  long int_mult;
  int shaping_is_bipolar;
  int is;

  i_shape_sig_ptr = GET_INPUT_PTR( I_Shape_Sig );
  q_shape_sig_ptr = GET_INPUT_PTR( Q_Shape_Sig );

  samps_out_cnt = Samps_Out_Cnt;
  samp_intvl = Samp_Intvl;
  pi_over_bit_dur = Pi_Over_Bit_Dur;
  shaping_is_bipolar = Shaping_Is_Bipolar;

  for (is=0; is<Block_Size; is++)
    {
    argument = pi_over_bit_dur * samps_out_cnt * samp_intvl;
    int_mult = long(argument/TWO_PI);
    argument -= int_mult*TWO_PI;
    //cout << "arg = " << argument << endl;

    if(shaping_is_bipolar)
      {
      *i_shape_sig_ptr = sin(argument);
      *q_shape_sig_ptr = cos(argument);
      }
    else
      {
      *i_shape_sig_ptr = fabs(sin(argument));
      *q_shape_sig_ptr = fabs(cos(argument));
      }
    samps_out_cnt++;
    i_shape_sig_ptr++;
    q_shape_sig_ptr++;
    }
  Samps_Out_Cnt = samps_out_cnt;
  return(_MES_AOK);
}

