//
//  File = discrete_delay_T.cpp
//

#include <stdlib.h>
#include <strstream>
#include "parmfile.h"
#include "model_error.h"
#include "discrete_delay_T.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;
extern int PassNumber;
using namespace std;

//======================================================
// general constructor that supports any of the possible 
// delay modes

template< class T >
DiscreteDelay< T >::DiscreteDelay( 
                  char* instance_name,
                  PracSimModel* outer_model,
                  Signal<T>* in_sig,
                  Signal<T>* out_sig,
                  Control<int> *dynam_dly_ctl,
                  Control<bool> *delay_chg_enab_ctl )
           :PracSimModel( instance_name,
                          outer_model)
{  
   MODEL_NAME(DiscreteDelay_T);
   this->Constructor_Common_Tasks( instance_name, 
                                   in_sig, out_sig);
   //---------------------------
   //  Controls

   Dynam_Dly_Ctl = dynam_dly_ctl;
   Delay_Chg_Enab_Ctl = delay_chg_enab_ctl;
   return;
}
//======================================================
//  Constructor 2
//  This constructor supports any delay mode except 
//  DELAY_MODE_GATED (The calling sequence does not 
//  pass the gating control.)

template< class T >
DiscreteDelay< T >::DiscreteDelay( 
                          char* instance_name,
                          PracSimModel* outer_model,
                          Signal<T>* in_signal,
                          Signal<T>* out_signal,
                          Control<int> *dynam_dly_ctl )
              :PracSimModel(instance_name,
                            outer_model)
{  
   this->Constructor_Common_Tasks(  instance_name, 
                                    in_signal, 
                                    out_signal);

   //------------------------------------------
   //  Controls

   Dynam_Dly_Ctl = dynam_dly_ctl;

   switch (Delay_Mode){
    case DELAY_MODE_NONE:
    case DELAY_MODE_FIXED:
    case DELAY_MODE_DYNAMIC:
       break;
    case DELAY_MODE_GATED:
       ostrstream temp_stream;
       temp_stream << "DELAY_MODE_GATED is not " 
               << "supported by called constructor (2)"
               << ends;
       char *message = temp_stream.str();
       PsModelError(FATAL, message);
       delete []message;
       break;
   }
   return;
}
//======================================================
//  Constructor 3
//  This constructor supports only DELAY_MODE_NONE and 
//  DELAY_MODE_FIXED  (The calling sequence does not 
//  pass the delay control or the gating control.)

template< class T >
DiscreteDelay< T >
            ::DiscreteDelay( char* instance_name,
                             PracSimModel* outer_model,
                             Signal<T>* in_signal,
                             Signal<T>* out_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
   this->Constructor_Common_Tasks(  instance_name, 
                                    in_signal, 
                                    out_signal);

   char *message;
   ostrstream temp_stream;

   switch (Delay_Mode){
    case DELAY_MODE_NONE:
    case DELAY_MODE_FIXED:
       break;
    case DELAY_MODE_DYNAMIC:
       temp_stream << "DELAY_MODE_DYNAMIC is not " 
          << "supported by called contructor (3)"
          << ends;
       message = temp_stream.str();
       PsModelError(FATAL, message);
       delete []message;
       break;
    case DELAY_MODE_GATED:
       temp_stream << "DELAY_MODE_GATED is not " 
          << "supported by called constructor (3)"
          << ends;
       message = temp_stream.str();
       PsModelError(FATAL, message);
       delete []message;
       break;
   }
   return;
}
//======================================================
template< class T >
void DiscreteDelay< T >
      ::Constructor_Common_Tasks( char* instance_name,
                                  Signal<T>* in_signal,
                                  Signal<T>* out_signal)
{
   MODEL_NAME(DiscreteDelay);
   ActiveModel = this;

   //-----------------------------------
   // Read configuration parameters
   OPEN_PARM_BLOCK;

   Delay_Mode = GetDelayModeParm("Delay_Mode\0");
   BasicResults << "   " << "Delay_Mode = " 
                << Delay_Mode << endl;

   GET_INT_PARM(Initial_Delay_In_Samps);
   Dynamic_Delay_Bias = 0;

   switch (Delay_Mode){
    case DELAY_MODE_NONE:
    case DELAY_MODE_FIXED:
       Max_Delay_In_Samps = Initial_Delay_In_Samps;
       Num_Initial_Passes = 0;
       break;
    case DELAY_MODE_DYNAMIC:
    case DELAY_MODE_GATED:
       GET_INT_PARM(Dynamic_Delay_Bias);
       GET_INT_PARM(Max_Delay_In_Samps);
       GET_INT_PARM(Num_Initial_Passes);
       break;
   }

   //-----------------------------------
   //  Signals

   In_Sig = in_signal;
   Out_Sig = out_signal;

   MAKE_OUTPUT( Out_Sig );
   MAKE_INPUT( In_Sig );
};
//======================================================
template< class T >
DiscreteDelay<T>::~DiscreteDelay( void ){ };

//======================================================

template< class T >
void DiscreteDelay<T>::Initialize()
{
   //---------------------------------------------------
   //  Initialize derived parameters

   Samp_Intvl = In_Sig->GetSampIntvl();
   Block_Size = In_Sig->GetBlockSize();
   //---------------------------------------------------
   //  Initialize physical buffer

   Max_Buffer_Len = Max_Delay_In_Samps;

   Start_Of_Buffer = new T[Max_Buffer_Len];
   for(int i=0; i<Max_Buffer_Len; i++){
      *(Start_Of_Buffer+i) = 0;
   }
   //---------------------------------------------------
   //  Initialize active portion of buffer

   Active_Delay_In_Samps = Initial_Delay_In_Samps;

   End_Of_Buffer = Start_Of_Buffer + Max_Buffer_Len - 1;
   Write_Ptr = Start_Of_Buffer;
   Read_Ptr = Start_Of_Buffer + Max_Buffer_Len - 
                                 Active_Delay_In_Samps;

}
//======================================================
template< class T >
int DiscreteDelay<T>::Execute()
{
  T *start_of_buffer, *end_of_buffer;
  T *read_ptr, *write_ptr;
  T input_samp, *input_signal_ptr;
  T output_samp, *output_signal_ptr;
  int is, block_size;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);
  block_size = In_Sig->GetValidBlockSize();
  Out_Sig->SetValidBlockSize(block_size);

  //------------------------------------------------
  //  Do actions peculiar to each delay mode

   switch (Delay_Mode){
   case DELAY_MODE_NONE:
      for(is=0; is<Block_Size; is++){
         input_samp = *input_signal_ptr++;
         *output_signal_ptr++ =input_samp;
      }
      return(_MES_AOK);
   case DELAY_MODE_FIXED:
      break;
   case DELAY_MODE_GATED:
      //  If delay change is NOT enabled, get out of
      //  switch.  If delay change IS enabled, fall 
      //  through to next case and get new value for 
      //  Active_Delay.

      if( Delay_Chg_Enab_Ctl->GetValue() == false ){
         break;
      }
   case DELAY_MODE_DYNAMIC:
      if( PassNumber > Num_Initial_Passes ){
         Active_Delay_In_Samps = 
                        Dynam_Dly_Ctl->GetValue();
         Active_Delay_In_Samps += Dynamic_Delay_Bias;
         Read_Ptr = Write_Ptr - Active_Delay_In_Samps;

         if(Read_Ptr < Start_Of_Buffer) Read_Ptr += 
                                    Max_Buffer_Len;
      }
      break;
   } // end of switch on Delay_Mode

   //------------------------------------------------
   //  copy frequently used items into local storage

   block_size = Block_Size;
   read_ptr = Read_Ptr;
   write_ptr = Write_Ptr;
   start_of_buffer = Start_Of_Buffer;
   end_of_buffer = End_Of_Buffer;

   //---------------------------------------------------
   // if active delay is zero, just copy input to output

   if(Active_Delay_In_Samps == 0){
      for(is=0; is<block_size; is++){
         input_samp = *input_signal_ptr++;
         *output_signal_ptr++ =input_samp;
      }
      return(_MES_AOK);
   }
  //---------------------------------------------------
  //  error condition if active delay exceeds max delay

   if(Active_Delay_In_Samps > Max_Delay_In_Samps){
      ostrstream temp_stream;
      temp_stream 
            << "Active_Delay_In_Samps (" 
            << Active_Delay_In_Samps
            << ") is greater than Max_Delay_In_Samps ("
            << Max_Delay_In_Samps << ")." << endl;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
   }

   //  normal processing loop
   for(is=0; is<block_size; is++){
      input_samp = *input_signal_ptr++;

      // get output samp from delay buffer    
      output_samp = *read_ptr++;
      if(read_ptr > end_of_buffer) read_ptr = 
                                    start_of_buffer;

      // put input sample into delay buffer
      *write_ptr++ = input_samp;
      if(write_ptr > end_of_buffer) write_ptr = 
                                    start_of_buffer;

      *output_signal_ptr++ =output_samp;
   } // end of main loop

   Read_Ptr = read_ptr;
   Write_Ptr = write_ptr;
   return(_MES_AOK);
}
template DiscreteDelay< int >;
template DiscreteDelay< float >;
template DiscreteDelay< bit_t >;

