//
//  File = downsampler_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "model_error.h"
#include "downsampler_T.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;
extern int PassNumber;

//======================================================================
//  Constructor

template< class T >
Downsampler< T >::Downsampler( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
  MODEL_NAME(Downsampler);
  ENABLE_MULTIRATE;

//  ActiveModel = this;

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  GET_INT_PARM(Decim_Rate);

  //-----------------------------------
  //  Signals

  In_Sig = in_signal;
  Out_Sig = out_signal;
  double resamp_rate;
  resamp_rate = 1.0/Decim_Rate;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );
  CHANGE_RATE(In_Sig, Out_Sig, resamp_rate);
};
//================================================
template< class T >
Downsampler<T>::~Downsampler( void ){ };

//=======================================================

template< class T >
void Downsampler<T>::Initialize()
  {
  //---------------------------------------
  //  Initialize derived parameters

  Samp_Intvl = In_Sig->GetSampIntvl();
  //Block_Size = In_Sig->GetBlockSize();
  Num_Opening_Drops = 0;

}

//=======================================
template< class T >
int Downsampler<T>::Execute()
{
  T input_samp, *input_signal_ptr;
  T *output_signal_ptr;
  int is, block_size;
  int out_block_size;
  int new_num_opening_drops;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  block_size = In_Sig->GetValidBlockSize();
  out_block_size = (block_size - Num_Opening_Drops)/Decim_Rate;
   new_num_opening_drops = block_size - Num_Opening_Drops - out_block_size * Decim_Rate;
   if(new_num_opening_drops != 0) out_block_size++;

  Out_Sig->SetValidBlockSize( out_block_size );


  //-----------------------------------------------------
  //  normal processing loop

   for(is=0; is<out_block_size; is++) {
      input_samp = *input_signal_ptr++;
      *output_signal_ptr++ = input_samp;
      for(int j=1; j<Decim_Rate; j++) {
         input_signal_ptr++;
      }
   }
   Num_Opening_Drops = new_num_opening_drops;
   return(_MES_AOK);

}
template Downsampler< int >;
template Downsampler< float >;
template Downsampler< bit_t >;

