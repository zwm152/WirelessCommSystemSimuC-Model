//
//  File = contin_delay2_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "sigplot.h"
#include "model_error.h"
#include "contin_delay2_T.h"
#include "model_graph.h"
#include "complex_io.h"
#include "sinc.h"
extern SignalPlotter SigPlot;
extern int PassNumber;
extern int EnclaveNumber;
extern int EnclaveOffset[10];
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern PracSimModel *ActiveModel;

template< class T >
int ContinuousDelay2< T >::Instance_Count=0;

//==================================================================
// general constructor that supports any of the possible delay modes

template< class T >
ContinuousDelay2< T >::ContinuousDelay2( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal,
                                Control<float> *new_delay,
                                Control<bool> *delay_change_enabled )
              :PracSimModel(instance_name,
                            outer_model)
{  
  this->Constructor_Common_Tasks( instance_name, in_signal, out_signal);
  //------------------------------------------
  //  Controls

  New_Delay = new_delay;
  Delay_Change_Enabled = delay_change_enabled;

  return;
  }
//==================================================================
//  Constructor 2
//  This constructor supports any delay mode except DELAY_MODE_GATED
//  (The calling sequence does not pass the gating control.)

template< class T >
ContinuousDelay2< T >::ContinuousDelay2( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal,
                                Control<float> *new_delay )
              :PracSimModel(instance_name,
                            outer_model)
{  
  this->Constructor_Common_Tasks( instance_name, in_signal, out_signal);
  //------------------------------------------
  //  Controls

  New_Delay = new_delay;

  switch (Delay_Mode)
    {
    case DELAY_MODE_NONE:
    case DELAY_MODE_FIXED:
    case DELAY_MODE_DYNAMIC:
      break;
    case DELAY_MODE_GATED:
      {
      ostrstream temp_stream;
      temp_stream << "DELAY_MODE_GATED is not supported by called constructor (2)"
                   << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
      }
      break;
    }
  return;
  }
//======================================================================
//  Constructor 3
//  This constructor supports only DELAY_MODE_NONE and DELAY_MODE_FIXED
//  (The calling sequence does not pass the delay control or 
//   the gating control.)

template< class T >
ContinuousDelay2< T >::ContinuousDelay2( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
  this->Constructor_Common_Tasks( instance_name, in_signal, out_signal);

  switch (Delay_Mode)
    {
    case DELAY_MODE_NONE:
    case DELAY_MODE_FIXED:
      break;
    case DELAY_MODE_DYNAMIC:
      {
      ostrstream temp_stream;
      temp_stream << "DELAY_MODE_DYNAMIC is not supported by called contructor (3)"
                   << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
      }
      break;
    case DELAY_MODE_GATED:
      {
      ostrstream temp_stream;
      temp_stream << "DELAY_MODE_GATED is not supported by called constructor (3)"
                   << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
      }
      break;
    }
  return;
  }
//===================================================================
template< class T >
void ContinuousDelay2< T >::Constructor_Common_Tasks( char* instance_name,
                                              Signal<T>* in_signal,
                                              Signal<T>* out_signal)
{
   MODEL_NAME(ContinuousDelay);
   ActiveModel = this;

   Instance_Count++;
   Instance_Number = Instance_Count;
  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  Delay_Mode = GetDelayModeParm("Delay_Mode\0");
  BasicResults << "   " << "Delay_Mode = " << Delay_Mode << endl;

  Interp_Mode = GetInterpModeParm("Interp_Mode\0");
  BasicResults << "   " << "Interp_Mode = " << Interp_Mode << endl;

  if( Interp_Mode == INTERP_MODE_SINC )
    {
    GET_INT_PARM(Num_Sidelobes);
    }
  else
    {
    Num_Sidelobes = 0;
    }

  GET_DOUBLE_PARM(Max_Delay);

  GET_DOUBLE_PARM(Initial_Delay);

  //-----------------------------------
  //  Signals

  In_Sig = in_signal;
  Out_Sig = out_signal;

  MAKE_INPUT( In_Sig );
  //EnclaveNumber++; // must come after MAKE_INPUT and before MAKE_OUTPUT
  MAKE_OUTPUT( Out_Sig );
};
//================================================
template< class T >
ContinuousDelay2<T>::~ContinuousDelay2( void ){ };

//=======================================================

template< class T >
void ContinuousDelay2<T>::Initialize()
{
  double active_delay_in_samps;
  double sinc_offset;
  int idx;

  //---------------------------------------
  //  Initialize derived parameters

  Samp_Intvl = In_Sig->GetSampIntvl();
  Nom_Block_Size = In_Sig->GetBlockSize();
  //---------------------------------------
  //  Initialize physical buffer

  Blocks_Of_Offset = 1;

  //---------------------------------------
  //  Initialize active portion of buffer

  Active_Delay = Initial_Delay;
  Longest_Active_Delay = Initial_Delay;
  sinc_offset = ceil(Active_Delay/Samp_Intvl) - (Active_Delay/Samp_Intvl);
  Offset_Sum_Start_To_Out_Samp = Num_Sidelobes + int(ceil(Active_Delay/Samp_Intvl))-1;
  //Offset_Sum_Start_To_Out_Samp = Num_Sidelobes + int(ceil(Active_Delay/Samp_Intvl));
  //Max_Buffer_Len = Nom_Block_Size + Offset_Sum_Start_To_Out_Samp+1;
  Max_Buffer_Len = 2*Nom_Block_Size;

  Start_Of_Buffer = new T[Max_Buffer_Len];
  for(int i=0; i<Max_Buffer_Len; i++)
    {
    //*(Start_Of_Buffer+i) = 1e6;
    *(Start_Of_Buffer+i) = 0.0;
    }

  active_delay_in_samps = Active_Delay/Samp_Intvl;
  Delay_Ceil = int(ceil(active_delay_in_samps));
  Noncausal_Samps = Num_Sidelobes - Delay_Ceil;
  if(Noncausal_Samps < 0) Noncausal_Samps=0;
  Longest_Noncausal_Samps = Noncausal_Samps;
  if(Active_Delay == 0) Longest_Noncausal_Samps = 0;

  Preamb_Len = Delay_Ceil - Num_Sidelobes;
  Preamb_Samps_Remaining = Preamb_Len;
  Preamb_Blocks_Remaining = int(ceil(Preamb_Len/Nom_Block_Size));
  Active_Buffer_Len = 1 + int(floor(active_delay_in_samps));

  Interp_Weight = active_delay_in_samps - floor(active_delay_in_samps); //correct one
  if(Interp_Weight > 0.999999) Interp_Weight = 1.0;
  if(Interp_Weight < 0.000001) Interp_Weight = 0.0;
  One_Minus_Weight = 1.0 - Interp_Weight;

  End_Of_Buffer = Start_Of_Buffer + Max_Buffer_Len - 1;
  Write_Ptr = Start_Of_Buffer;
  //Write_Ptr = End_Of_Buffer;
  Read_Ptr_Start = Start_Of_Buffer + Max_Buffer_Len - Offset_Sum_Start_To_Out_Samp;
  //Read_Ptr_Start = Start_Of_Buffer;
  Num_Blocks_Skipped = 0;

  Sinc_Val = new float[2*Num_Sidelobes];
//  if(sinc_offset==0.0)
//  {
 //     Sinc_Val
//  }
//  else
//  {
     double sum = 0.0;
     double sum_sqrs = 0.0;
     for(idx=0; idx < 2*Num_Sidelobes; idx++)
       {
       Sinc_Val[idx] = float(sinc(Num_Sidelobes - 1 - idx + sinc_offset));
       sum += Sinc_Val[idx];
       sum_sqrs += Sinc_Val[idx] * Sinc_Val[idx];
       //Sinc_Val[idx] = 1.0/float(2*Num_Sidelobes);
       }
     Sinc_Val[0] = float(0.6*Sinc_Val[0]);
     Sinc_Val[2*Num_Sidelobes-1] = float(0.6*Sinc_Val[2*Num_Sidelobes-1]);
//  }

}

//=======================================
template< class T >
int ContinuousDelay2<T>::Execute()
{
  T *start_of_buffer, *end_of_buffer;
  T *read_ptr, *write_ptr;
  T input_samp, *input_signal_ptr;
  T output_samp, *output_signal_ptr;
  T left_samp, right_samp;
  T sum;
  T zero_T;
  T *oldest_window_loc_ptr;
  double actv_dly_in_samps;
  int num_sidelobes;
  int max_buf_len;
  int is, input_block_size;
  int output_block_size;
  double samp_intvl;
  double sinc_offset;
  float *sinc_val;
  float interp_weight, one_minus_weight;
  int avail_output_len;
  int output_idx;
  int idx;
  int output_block_start;
  int old_offset;
  bool noncausal_samps_has_increased;
  bool debug_flag;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  input_block_size = In_Sig->GetValidBlockSize();
  //valid_block_size = In_Sig->GetValidBlockSize();

  //------------------------------------------------
  //  Do actions peculiar to each delay mode

  switch (Delay_Mode)
    {
    //- - - - - - - - - - - - - - - - - - - - - - 
    case DELAY_MODE_NONE:
    case DELAY_MODE_FIXED:
      {
      break;
      }
    //- - - - - - - - - - - - - - - - - - - - - - 
    case DELAY_MODE_GATED:
      {
      //  If delay change is NOT enabled, get out of switch.
      //  If delay change IS enabled, fall through to next case
      //  and get new value for Active_Delay.

      if( Delay_Change_Enabled->GetValue() == false )
        {
        break;
        }
      }
    //- - - - - - - - - - - - - - - - - - - - - - 
    case DELAY_MODE_DYNAMIC:
      {
      actv_dly_in_samps = 0.0;
      Active_Delay = New_Delay->GetValue();
      noncausal_samps_has_increased = false;
      if(Active_Delay > Longest_Active_Delay)
      {
         actv_dly_in_samps = Active_Delay/Samp_Intvl;
         Delay_Ceil = int(ceil(actv_dly_in_samps));
         Noncausal_Samps = Num_Sidelobes - Delay_Ceil;
         if(Noncausal_Samps > Longest_Noncausal_Samps)
         {
            //if(Noncausal_Samps < 0) Noncausal_Samps=0;
            Longest_Noncausal_Samps = Noncausal_Samps;
            noncausal_samps_has_increased = true;

            old_offset = Offset_Sum_Start_To_Out_Samp;
            Offset_Sum_Start_To_Out_Samp = Num_Sidelobes 
                        + int(ceil(Active_Delay/Samp_Intvl))-1;
//            Read_Ptr_Start = Start_Of_Buffer 
//                        + Max_Buffer_Len - Offset_Sum_Start_To_Out_Samp;
            Read_Ptr_Start += old_offset - Offset_Sum_Start_To_Out_Samp;
            if(Read_Ptr_Start < Start_Of_Buffer) Read_Ptr_Start += Max_Buffer_Len;
            if(Read_Ptr_Start > End_Of_Buffer) Read_Ptr_Start -= Max_Buffer_Len;
         }
         Longest_Active_Delay = Active_Delay;
      }

      sinc_offset = ceil(Active_Delay/Samp_Intvl) - (Active_Delay/Samp_Intvl);
      for(idx=0; idx < 2*Num_Sidelobes; idx++)
      {
         Sinc_Val[idx] = float(sinc(Num_Sidelobes - 1 - idx + sinc_offset));
      }
      Sinc_Val[0] = float(0.6*Sinc_Val[0]);
      Sinc_Val[2*Num_Sidelobes-1] = float(0.6*Sinc_Val[2*Num_Sidelobes-1]);

      double active_delay_in_samps = Active_Delay/Samp_Intvl;
      Active_Buffer_Len = 1 + int(floor(active_delay_in_samps));
      Interp_Weight = active_delay_in_samps - floor(active_delay_in_samps);
      if(Interp_Weight > 0.999999) Interp_Weight = 1.0;
      if(Interp_Weight < 0.000001) Interp_Weight = 0.0;
      One_Minus_Weight = 1.0 - Interp_Weight;

      break;
      }
    } // end of switch on Delay_Mode

  //------------------------------------------------
  //  copy frequently used items into local storage

  samp_intvl = Samp_Intvl;
  write_ptr = Write_Ptr;
  start_of_buffer = Start_Of_Buffer;
  end_of_buffer = End_Of_Buffer;
  interp_weight = Interp_Weight;
  one_minus_weight = One_Minus_Weight;
  sinc_val = Sinc_Val;

  //---------------------------------------------------
  //  error condition if active delay exceeds max delay

  if(Active_Delay > Max_Delay)
    {
    ostrstream temp_stream;
    temp_stream << "Active_Delay (" << Active_Delay
                 << ") is greater than Max_Delay ("
                 << Max_Delay << ")." << ends;
    char *message = temp_stream.str();
    PsModelError(FATAL, message);
    delete []message;

    }

  //--------------------------------------------------
  //  normal sample processing

  switch (Interp_Mode)
    {
    case INTERP_MODE_LINEAR:
      for(is=0; is<input_block_size; is++)
        {
        input_samp = *input_signal_ptr++;

        //---------------------------------------------------------------
        // get samps that bracket desired time instant from delay buffer
  
        left_samp = *read_ptr++;
        if(read_ptr > end_of_buffer) read_ptr = start_of_buffer;
        if(Active_Delay < Samp_Intvl)
          right_samp = input_samp;
        else
          right_samp = *read_ptr;

        //------------------------------------------
        // do interpolation to get output value

        output_samp = interp_weight*left_samp + one_minus_weight*right_samp;

        //------------------------------------
        // put input sample into delay buffer

        *write_ptr++ = input_samp;
        if(write_ptr > end_of_buffer) write_ptr = start_of_buffer;

        *output_signal_ptr++ =output_samp;
        }
      break;
    case INTERP_MODE_SINC:
      num_sidelobes = Num_Sidelobes;
      max_buf_len = Max_Buffer_Len;
      if( (PassNumber == 1) || noncausal_samps_has_increased)
      {
        //----------------------------------
        // preload enough input samples to
        // accomodate non-causal tail of sinc window
        for(is=0; is<Noncausal_Samps; is++)
        {
          input_samp = *input_signal_ptr++;
          if(input_samp > 100.0)
          {
          debug_flag = true;
          }
          *write_ptr++ = input_samp;
          if(write_ptr > end_of_buffer) 
          {
            write_ptr = start_of_buffer;
          }
        }
        output_block_size = input_block_size - Noncausal_Samps;
        output_block_start = Noncausal_Samps;
      }
      else
      {
        output_block_size = input_block_size;
        output_block_start = 0;
      }
         if((Instance_Number == 2) && (PassNumber >= 10)) (*DebugFile)<<"NEW BLOCK" << endl;
      for(is=0; is<output_block_size; is++)
        {
        input_samp = *input_signal_ptr++;
          if(input_samp > 100.0)
          {
          debug_flag = true;
          }
        //------------------------------------
        // put input sample into delay buffer

        *write_ptr++ = input_samp;
        if(write_ptr > end_of_buffer) 
          {
          write_ptr = start_of_buffer;
          }
        //----------------------------------
        read_ptr = Read_Ptr_Start;
        oldest_window_loc_ptr = Read_Ptr_Start;
        Read_Ptr_Start++;
        if(Read_Ptr_Start > end_of_buffer) Read_Ptr_Start = start_of_buffer;

        sum = 0.0;
        zero_T = 0.0;
        for(int sum_idx=0; sum_idx< 2*Num_Sidelobes; sum_idx++)
          {
          sum += (*read_ptr) * sinc_val[sum_idx];
          read_ptr++;
          if(read_ptr > end_of_buffer) read_ptr = start_of_buffer;
          }
        //*oldest_window_loc_ptr = 10e6;
        *output_signal_ptr++ = sum;
        avail_output_len--;
        output_idx++;

        }
      Out_Sig->SetValidBlockSize(output_block_size);
      *DebugFile << "delayed output block size = " << output_block_size << endl;
      break;
  default:
    ostrstream temp_stream;
    temp_stream << "Requested interpolation mode is not supported"
                 << ends;
    char *message = temp_stream.str();
    PsModelError(FATAL, message);
    delete []message;
  } // end of switch om Interp_Mode
//   }
//   *(output_signal_ptr-1)=50.0;

  Write_Ptr = write_ptr;
  return(_MES_AOK);
}
//template ContinuousDelay2< int >;
template ContinuousDelay2< float >;
//template ContinuousDelay2< std::complex<float> >;