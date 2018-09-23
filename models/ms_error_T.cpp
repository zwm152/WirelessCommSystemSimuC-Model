//
//  File = ms_error_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "model_error.h"
#include "ms_error_T.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;
extern int PassNumber;

//======================================================================
//  Constructor

template< class T >
MeanSquareError< T >::MeanSquareError( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* ref_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
  MODEL_NAME(MeanSquareError);
  //ENABLE_MULTIRATE;

//  ActiveModel = this;

  //-----------------------------------
  // Read configuration parameters
  //OPEN_PARM_BLOCK;

  //GET_INT_PARM(Interp_Rate);

  //-----------------------------------
  //  Signals

  In_Sig = in_signal;
  Ref_Sig = ref_signal;

  MAKE_INPUT( Ref_Sig );
  MAKE_INPUT( In_Sig );
  //CHANGE_RATE(In_Sig, Out_Sig, Interp_Rate);
};
//================================================
template< class T >
MeanSquareError<T>::~MeanSquareError( void ){ };

//=======================================================

template< class T >
void MeanSquareError<T>::Initialize()
  {
  //---------------------------------------
  //  Initialize derived parameters

  //Samp_Intvl = In_Sig->GetSampIntvl();
  //Block_Size = In_Sig->GetBlockSize();
  Cumul_Ms_Error = 0.0;
  Cumul_Ref_Pwr = 0.0;

}

//=======================================
template< class T >
int MeanSquareError<T>::Execute()
{
  T *input_signal_ptr;
  T *ref_signal_ptr;
  int is, block_size;
  double block_sum;
  double ms_error, rms_error;
  double cumul;
  double input_samp, input_pwr;
  double ref_samp, ref_pwr;
  double scale_fact;
  double snr;
  T diff;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  ref_signal_ptr = GET_INPUT_PTR(Ref_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  block_size = In_Sig->GetValidBlockSize();
  //Out_Sig->SetValidBlockSize(block_size * Interp_Rate);


  //-----------------------------------------------------
  //  normal processing loop

  block_sum = 0.0;
   input_pwr = 0.0;
   ref_pwr = 0.0;

   for(is=0; is<block_size; is++) {
      input_samp = (*input_signal_ptr++);
      input_pwr += input_samp*input_samp;
      }

   for(is=0; is<block_size; is++) {
      ref_samp = (*ref_signal_ptr++);
      ref_pwr += ref_samp*ref_samp;
      }
   Cumul_Ref_Pwr += ref_pwr;

   scale_fact = sqrt(ref_pwr/input_pwr);
   scale_fact = 1.0;

  ref_signal_ptr = GET_INPUT_PTR(Ref_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

   for(is=0; is<block_size; is++) {
      diff = (scale_fact*(*input_signal_ptr++)) - (*ref_signal_ptr++);
      block_sum += diff * diff;
      }
   ms_error = block_sum/block_size;
   Cumul_Ms_Error += block_sum;
   rms_error = sqrt(ms_error);
   snr = 10.0* log10(ref_pwr/block_sum);
   cumul = 10.0* log10(Cumul_Ref_Pwr/Cumul_Ms_Error);
   if(PassNumber%50 == 0) {
      BasicResults << Instance_Name << ": " << PassNumber << " RMS = " << rms_error 
                  << "  SNR = " << snr << " Cumul = " << cumul << endl;
      BasicResults << " scale_fact = " << scale_fact << endl;
      }
   return(_MES_AOK);

}
//template MeanSquareError< int >;
template MeanSquareError< float >;
//template MeanSquareError< bit_t >;

