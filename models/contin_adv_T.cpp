//
//  File = contin_adv_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "model_error.h"
#include "contin_adv_T.h"
#include "model_graph.h"
#include "sinc.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//==================================================================
// general constructor that supports any of the possible delay modes

template< class T >
ContinuousAdvance< T >::ContinuousAdvance( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal,
                                Control<float> *new_advance,
                                Control<bool> *advance_change_enabled )
              :PracSimModel(instance_name,
                            outer_model)
{  
  this->Constructor_Common_Tasks( instance_name, in_signal, out_signal);
  //------------------------------------------
  //  Controls

  New_Advance = new_advance;
  Advance_Change_Enabled = advance_change_enabled;

  return;
  }
//==================================================================
//  Constructor 2
//  This constructor supports any delay mode except DELAY_MODE_GATED
//  (The calling sequence does not pass the gating control.)

template< class T >
ContinuousAdvance< T >::ContinuousAdvance( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal,
                                Control<float> *new_advance )
              :PracSimModel(instance_name,
                            outer_model)
{  
  this->Constructor_Common_Tasks( instance_name, in_signal, out_signal);
  //------------------------------------------
  //  Controls

  New_Advance = new_advance;

  switch (Advance_Mode)
    {
    case ADVANCE_MODE_NONE:
    case ADVANCE_MODE_FIXED:
    case ADVANCE_MODE_DYNAMIC:
      break;
    case ADVANCE_MODE_GATED:
      {
      ostrstream temp_stream;
      temp_stream << "ADVANCE_MODE_GATED is not supported by called constructor (2)"
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
//  This constructor supports only ADVANCE_MODE_NONE and ADVANCE_MODE_FIXED
//  (The calling sequence does not pass the delay control or 
//   the gating control.)

template< class T >
ContinuousAdvance< T >::ContinuousAdvance( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
  this->Constructor_Common_Tasks( instance_name, in_signal, out_signal);

  switch (Advance_Mode)
    {
    case ADVANCE_MODE_NONE:
    case ADVANCE_MODE_FIXED:
      break;
    case ADVANCE_MODE_DYNAMIC:
      {
      ostrstream temp_stream;
      temp_stream << "ADVANCE_MODE_DYNAMIC is not supported by called contructor (3)"
                   << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
      }
      break;
    case ADVANCE_MODE_GATED:
      {
      ostrstream temp_stream;
      temp_stream << "ADVANCE_MODE_GATED is not supported by called constructor (3)"
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
void ContinuousAdvance< T >::Constructor_Common_Tasks( char* instance_name,
                                              Signal<T>* in_signal,
                                              Signal<T>* out_signal)
{
  MODEL_NAME(ContinuousAdvance);
  ActiveModel = this;

  //-----------------------------------
  // Read configuration parameters
  OPEN_PARM_BLOCK;

  Advance_Mode = GetAdvanceModeParm("Advance_Mode\0");
  BasicResults << "   " << "Advance_Mode = " << Advance_Mode << endl;

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

  GET_DOUBLE_PARM(Max_Advance);

  GET_DOUBLE_PARM(Initial_Advance);

  //-----------------------------------
  //  Signals

  In_Sig = in_signal;
  Out_Sig = out_signal;

  MAKE_OUTPUT( Out_Sig );
  MAKE_INPUT( In_Sig );
};
//================================================
template< class T >
ContinuousAdvance<T>::~ContinuousAdvance( void ){ };

//=======================================================

template< class T >
void ContinuousAdvance<T>::Initialize()
  {
  double active_adv_in_samps;
  //---------------------------------------
  //  Initialize derived parameters

  Samp_Intvl = In_Sig->GetSampIntvl();
  Block_Size = In_Sig->GetBlockSize();
  //---------------------------------------
  //  Initialize physical buffer

  Max_Buffer_Len = 1 + Num_Sidelobes + int(Max_Advance / Samp_Intvl);

  Start_Of_Buffer = new T[Max_Buffer_Len];
  for(int i=0; i<Max_Buffer_Len; i++)
    {
    *(Start_Of_Buffer+i) = 0;
    }
  //---------------------------------------
  //  Initialize active portion of buffer

  Active_Advance = Initial_Advance;
  active_adv_in_samps = Active_Advance/Samp_Intvl;
  Active_Buffer_Len = 1 + int(floor(active_adv_in_samps));

  Interp_Weight = active_adv_in_samps - floor(active_adv_in_samps); //correct one
  if(Interp_Weight > 0.999999) Interp_Weight = 1.0;
  if(Interp_Weight < 0.000001) Interp_Weight = 0.0;
  One_Minus_Weight = 1.0 - Interp_Weight;

  End_Of_Buffer = Start_Of_Buffer + Max_Buffer_Len - 1;
  Write_Ptr = Start_Of_Buffer;
  Read_Ptr = Start_Of_Buffer + Max_Buffer_Len - Active_Buffer_Len;

}

//=======================================
template< class T >
int ContinuousAdvance<T>::Execute()
{
  T *start_of_buffer, *end_of_buffer;
  T *read_ptr, *write_ptr;
  T input_samp, *input_signal_ptr;
  T output_samp, *output_signal_ptr;
  T *oldest_lead_samp_ptr, *newest_lag_samp_ptr;
  T *samp_ptr;
  T left_samp, right_samp;
  int num_sidelobes;
  int max_buffer_len;
  int is, block_size;
  double samp_intvl;
  float interp_weight, one_minus_weight;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  //------------------------------------------------
  //  Do actions peculiar to each delay mode

  switch (Advance_Mode)
    {
    //- - - - - - - - - - - - - - - - - - - - - - 
    case ADVANCE_MODE_NONE:
      {
      //...copy input directly to output
      for(is=0; is<Block_Size; is++)
        {
        input_samp = *input_signal_ptr++;
        *output_signal_ptr++ =input_samp;
        }
      return(_MES_AOK);
      }
    //- - - - - - - - - - - - - - - - - - - - - - 
    case ADVANCE_MODE_FIXED:
      {
      break;
      }
    //- - - - - - - - - - - - - - - - - - - - - - 
    case ADVANCE_MODE_GATED:
      {
      //  If advance change is NOT enabled, get out of switch.
      //  If advance change IS enabled, fall through to next case
      //  and get new value for Active_Delay.

      if( Advance_Change_Enabled->GetValue() == false )
        {
        break;
        }
      }
    //- - - - - - - - - - - - - - - - - - - - - - 
    case ADVANCE_MODE_DYNAMIC:
      {
      Active_Advance = New_Advance->GetValue();
      double active_adv_in_samps = Active_Advance/Samp_Intvl;
      Active_Buffer_Len = 1 + int(floor(active_adv_in_samps));
      Interp_Weight = active_adv_in_samps - floor(active_adv_in_samps);
      if(Interp_Weight > 0.999999) Interp_Weight = 1.0;
      if(Interp_Weight < 0.000001) Interp_Weight = 0.0;
      One_Minus_Weight = 1.0 - Interp_Weight;

      Read_Ptr = Write_Ptr - Active_Buffer_Len;
      if(Read_Ptr < Start_Of_Buffer) Read_Ptr += Max_Buffer_Len;

      break;
      }
    } // end of switch on Delay_Mode

  //------------------------------------------------
  //  copy frequently used items into local storage

  block_size = Block_Size;
  samp_intvl = Samp_Intvl;
  read_ptr = Read_Ptr;
  write_ptr = Write_Ptr;
  start_of_buffer = Start_Of_Buffer;
  end_of_buffer = End_Of_Buffer;
  interp_weight = Interp_Weight;
  one_minus_weight = One_Minus_Weight;

  //-----------------------------------------------------
  // if active delay is zero, just copy input to output

  if(Active_Advance == 0)
    {
    for(is=0; is<block_size; is++)
      {
      input_samp = *input_signal_ptr++;
      *output_signal_ptr++ =input_samp;
      }
    return(_MES_AOK);
    }

  //---------------------------------------------------
  //  error condition if active delay exceeds max delay

  if(Active_Advance > Max_Advance)
    {
    ostrstream temp_stream;
    temp_stream << "Active_Advance (" << Active_Advance
                 << ") is greater than Max_Advance ("
                 << Max_Advance << ")." << ends;
    char *message = temp_stream.str();
    PsModelError(FATAL, message);
    delete []message;

    }

  //--------------------------------------------------
  //  normal sample processing

  switch (Interp_Mode)
    {
    case INTERP_MODE_LINEAR:
      for(is=0; is<block_size; is++)
        {
        input_samp = *input_signal_ptr++;

        //---------------------------------------------------------------
        // get samps that bracket desired time instant from delay buffer
    
        left_samp = *read_ptr++;
        if(read_ptr > end_of_buffer) read_ptr = start_of_buffer;
        if(Active_Advance < Samp_Intvl)
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
      max_buffer_len = Max_Buffer_Len;
      for(is=0; is<block_size; is++)
        {
        input_samp = *input_signal_ptr++;

        //---------------------------------------------------------------
        // get samps that bracket desired time instant from delay buffer
    
        oldest_lead_samp_ptr = read_ptr - num_sidelobes + 1;
        if(oldest_lead_samp_ptr < start_of_buffer) oldest_lead_samp_ptr += max_buffer_len;
        left_samp = *read_ptr++;

        if(read_ptr > end_of_buffer) read_ptr = start_of_buffer;
        newest_lag_samp_ptr = read_ptr + max_buffer_len - 1;
        if(newest_lag_samp_ptr > end_of_buffer) newest_lag_samp_ptr -=max_buffer_len; 

        if(Active_Advance < Samp_Intvl)
          right_samp = input_samp;
        else
          right_samp = *read_ptr;

        //------------------------------------------
        // do interpolation to get output value

        output_samp = 0.0;
        int lobe_idx;

        //  Add in sidelobe contributions from leading samples

        samp_ptr = oldest_lead_samp_ptr;
        for(lobe_idx=num_sidelobes-1; lobe_idx>=0; lobe_idx--)
          {
          output_samp += (*samp_ptr++) * float(sinc(double(one_minus_weight + lobe_idx)));
          if(samp_ptr > end_of_buffer) samp_ptr = start_of_buffer;
          }

        //  Add in sidelobe contributions from leading samples

        for(lobe_idx=0; lobe_idx<num_sidelobes; lobe_idx++)
          {
          output_samp += float(sinc(double(interp_weight + lobe_idx))) * (*samp_ptr++);
          if(samp_ptr > end_of_buffer) samp_ptr = start_of_buffer;
          }

        //------------------------------------
        // put input sample into delay buffer

        *write_ptr++ = input_samp;
        if(write_ptr > end_of_buffer) write_ptr = start_of_buffer;

        *output_signal_ptr++ =output_samp;
        }
      break;
    default:
      ostrstream temp_stream;
      temp_stream << "Requested interpolation mode is not supported"
                   << ends;
      char *message = temp_stream.str();
      PsModelError(FATAL, message);
      delete []message;
    } // end of switch om Interp_Mode

  Read_Ptr = read_ptr;
  Write_Ptr = write_ptr;
  return(_MES_AOK);
}
template ContinuousAdvance< int >;
template ContinuousAdvance< float >;
template ContinuousAdvance< std::complex<float> >;