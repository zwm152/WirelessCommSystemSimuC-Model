//
//  File = upsampler_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "model_error.h"
#include "upsampler_T.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;
extern int PassNumber;

//======================================================================
//  Constructor

template< class T >
Upsampler< T >::Upsampler( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
  MODEL_NAME(Upsampler);
  ENABLE_MULTIRATE;

//  ActiveModel = this;

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  GET_INT_PARM(Interp_Rate);

  //-----------------------------------
  //  Signals

  In_Sig = in_signal;
  Out_Sig = out_signal;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );
  CHANGE_RATE(In_Sig, Out_Sig, Interp_Rate);
};
//================================================
template< class T >
Upsampler<T>::~Upsampler( void ){ };

//=======================================================

template< class T >
void Upsampler<T>::Initialize()
  {
  //---------------------------------------
  //  Initialize derived parameters

  //Samp_Intvl = In_Sig->GetSampIntvl();
  //Block_Size = In_Sig->GetBlockSize();

}

//=======================================
template< class T >
int Upsampler<T>::Execute()
{
  T input_samp, *input_signal_ptr;
  T *output_signal_ptr;
  int is, block_size;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size * Interp_Rate);


  //-----------------------------------------------------
  //  normal processing loop

   for(is=0; is<block_size; is++) {
      input_samp = *input_signal_ptr++;
      *output_signal_ptr++ = Interp_Rate * input_samp;
      for(int j=1; j<Interp_Rate; j++) {
         *output_signal_ptr++ = 0.0;
      }
   }
   return(_MES_AOK);

}
template Upsampler< int >;
template Upsampler< float >;
template Upsampler< bit_t >;

