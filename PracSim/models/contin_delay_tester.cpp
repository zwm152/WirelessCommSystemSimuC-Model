//
//  File = contin_delay_tester.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "contin_delay_tester.h"
#include "model_graph.h"
#include "uni_rand.h"
extern ParmFile ParmInput;
extern int PassNumber;

//======================================================

template< class T >
ContinuousDelayTester< T >::ContinuousDelayTester( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal,
                                Control<float> *delay_value,
                                Control<bool> *delay_change_enabled )
              :PracSimModel(instance_name,
                            outer_model)
{
  MODEL_NAME(ContinuousDelayTester);


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
ContinuousDelayTester<T>::~ContinuousDelayTester( void ){ };

//=======================================================

template< class T >
void ContinuousDelayTester<T>::Initialize()
{
  //---------------------------------------
  //  Initialize derived parameters

  Samp_Intvl = In_Sig->GetSampIntvl();
  //Block_Size = In_Sig->GetBlockSize();
}

//=======================================
template< class T >
int ContinuousDelayTester<T>::Execute()
{
  T input_samp, *input_signal_ptr;
  T *output_signal_ptr;
  int block_size;
  int is;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  for(is=0; is<block_size; is++)
    {
    input_samp = *input_signal_ptr++;
    //*output_signal_ptr++ = 0.5 * input_samp;
    *output_signal_ptr++ = input_samp;
    }

  if( (PassNumber < 10) && (PassNumber > 2))
    {
    float value = Delay_Value->GetValue();
    value += float(1.13);
    Delay_Value->SetValue(value);
    }
  if( ((PassNumber%2)==0) && (PassNumber > 2) )
    {
    Delay_Change_Enabled->SetValue(true);
    }
  else
    {
    Delay_Change_Enabled->SetValue(false);
    }
  return(_MES_AOK);

}
template ContinuousDelayTester< int >;
template ContinuousDelayTester< float >;
template ContinuousDelayTester< std::complex<float> >;

