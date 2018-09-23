//
//  File = disc_delay_tester.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "disc_delay_tester.h"
#include "model_graph.h"
#include "uni_rand.h"
extern ParmFile ParmInput;
extern int PassNumber;

//======================================================

template< class T >
DiscreteDelayTester< T >::DiscreteDelayTester( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal,
                                Control<int> *delay_value,
                                Control<bool> *delay_change_enabled )
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(DelayControlTester);

  //-----------------------------------
  // Read configuration parameters
  //OPEN_PARM_BLOCK;

  //GET_DOUBLE_PARM(Max_Delay);

  //GET_DOUBLE_PARM(Initial_Delay);

  //-----------------------------------
  //  Signals

  In_Sig = in_signal;
  Out_Sig = out_signal;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );

  //--------------------------------
  //  Controls

  Delay_Value = delay_value;
  Delay_Change_Enabled = delay_change_enabled;

  return;
  }
//================================================
template< class T >
DiscreteDelayTester<T>::~DiscreteDelayTester( void ){ };

//=======================================================

template< class T >
void DiscreteDelayTester<T>::Initialize()
{
  //---------------------------------------
  //  Initialize derived parameters

  Samp_Intvl = In_Sig->GetSampIntvl();
  Block_Size = In_Sig->GetBlockSize();
}

//=======================================
template< class T >
int DiscreteDelayTester<T>::Execute()
{
  T input_samp, *input_signal_ptr;
  T *output_signal_ptr;
  int is;
  int block_size;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);
  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  for(is=0; is<block_size; is++)
    {
    input_samp = *input_signal_ptr++;
    //*output_signal_ptr++ = 0.5 * input_samp;
    *output_signal_ptr++ = input_samp;
    }

  Delay_Value->SetValue(5-PassNumber);
  //Delay_Value->SetValue(4000);

  if( (PassNumber%2)==0 )
    {
    Delay_Change_Enabled->SetValue(true);
    }
  else
    {
    Delay_Change_Enabled->SetValue(false);
    }
  return(_MES_AOK);

}
template DiscreteDelayTester< int >;
template DiscreteDelayTester< float >;

