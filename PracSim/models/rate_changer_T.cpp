//
//  File = rate_changer_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <strstream>
#include "parmfile.h"
#include "sigplot.h"
#include "model_error.h"
#include "rate_changer_T.h"
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

//======================================================================
//  Constructor

template< class T >
RateChanger< T >::RateChanger( char* instance_name,
                                PracSimModel* outer_model,
                                Signal<T>* in_signal,
                                Signal<T>* out_signal )
              :PracSimModel(instance_name,
                            outer_model)
{  
   MODEL_NAME(RateChanger);
   ENABLE_MULTIRATE;
   ENABLE_CONST_INTERVAL;
   ActiveModel = this;

   //-----------------------------------
   // Read configuration parameters
   OPEN_PARM_BLOCK;

   GET_INT_PARM(Num_Sidelobes);

   GET_DOUBLE_PARM(Rate_Change_Factor);

   //-----------------------------------
   //  Signals

   In_Sig = in_signal;
   Out_Sig = out_signal;

   MAKE_INPUT( In_Sig );
   MAKE_OUTPUT( Out_Sig );

   // one sample per bit at input
   CHANGE_RATE( In_Sig, Out_Sig, Rate_Change_Factor );
  return;
  }
//================================================
template< class T >
RateChanger<T>::~RateChanger( void ){ };

//=======================================================

template< class T >
void RateChanger<T>::Initialize()
{
//  int idx;

  //---------------------------------------
  //  Initialize derived parameters

  Samp_Intvl = In_Sig->GetSampIntvl();
  Out_Samp_Intvl = Samp_Intvl/Rate_Change_Factor;
  Nom_Block_Size = In_Sig->GetBlockSize();
  //---------------------------------------
  //  Initialize physical buffer

  Max_Buffer_Len = 2*Nom_Block_Size;

  Start_Of_Buffer = new T[Max_Buffer_Len];
  for(int i=0; i<Max_Buffer_Len; i++)
    {
    *(Start_Of_Buffer+i) = 1e6;
    }

  End_Of_Buffer = Start_Of_Buffer + Max_Buffer_Len - 1;
  Write_Ptr = Start_Of_Buffer;

  Read_Ptr_Start = Start_Of_Buffer + Max_Buffer_Len - Num_Sidelobes;
  Rt_Brack_Avail = 0;

  Sinc_Val = new float[2*Num_Sidelobes];


}

//=======================================
template< class T >
int RateChanger<T>::Execute()
{
  T *start_of_buffer, *end_of_buffer;
  T *read_ptr, *write_ptr;
  T input_samp, *input_signal_ptr;
  T *output_signal_ptr;
  T *output_base;
  T sum;
  T zero_T;
  T *ptr_to_center;
  int num_sidelobes;
  int max_buf_len;
  int is, input_block_size;
  int output_block_size;
  double samp_intvl;
  float *sinc_val;
  int idx;
  double sinc_offset;
  double rt_brack_needed;
  double samp_inst;
  int num_inputs_avail;
  int rt_brack_avail;
  int beg_samp_count;
  int iflag;
  bool input_has_been_read;

  //-----------------------------------------
  // Get pointers for input and output signals
  
  output_signal_ptr = GET_OUTPUT_PTR(Out_Sig);
  output_base = GET_OUTPUT_PTR(Out_Sig);
  input_signal_ptr = GET_INPUT_PTR(In_Sig);

  input_block_size = In_Sig->GetValidBlockSize();
  *DebugFile << "input_block_size = " << input_block_size << endl;


  //------------------------------------------------
  //  copy frequently used items into local storage

  //block_size = Max_Block_Size;
  samp_intvl = Samp_Intvl;
  read_ptr = Read_Ptr;
  write_ptr = Write_Ptr;
  start_of_buffer = Start_Of_Buffer;
  end_of_buffer = End_Of_Buffer;
  sinc_val = Sinc_Val;

//    case INTERP_MODE_SINC:
      num_sidelobes = Num_Sidelobes;
      max_buf_len = Max_Buffer_Len;
      rt_brack_avail = Rt_Brack_Avail;
      num_inputs_avail = input_block_size;

      if(PassNumber == 1)
      {
        //----------------------------------
        // preload enough input samples to
        // accomodate non-causal tail of sinc window
        for(is=Num_Sidelobes; is>=0; is--)
        {
          *(end_of_buffer-is) = 0.0;
         // Buffer_Fullness++;
        }
        for(is=0; is<Num_Sidelobes; is++)
        {
          input_samp = *input_signal_ptr++;
          *write_ptr++ = input_samp;
          //Buffer_Fullness++;
          num_inputs_avail--;
        }
        Out_Samp_Count = 0;
        rt_brack_avail = 0;
      }

      //------------------------------------
      // processing for every pass

      beg_samp_count = Out_Samp_Count;

      while(num_inputs_avail >= 0)
      {
        samp_inst = Out_Samp_Count*Out_Samp_Intvl/Samp_Intvl;
        rt_brack_needed = ceil(samp_inst);

        if(rt_brack_needed <= rt_brack_avail)
        {
          if(samp_inst == rt_brack_needed)
          {
            // rt_bracket sample is the desired output
            // no interpolation needed
            ptr_to_center = Read_Ptr_Start + Num_Sidelobes;
            if(ptr_to_center > end_of_buffer) ptr_to_center -= Max_Buffer_Len;
            sum = *ptr_to_center;
          }
          else
          {
            // interploation is needed
            // desired output falls between rt_bracket and rt_bracket-1

            // generate sinc window

            sinc_offset = rt_brack_needed - samp_inst;
            for(idx=0; idx < 2*Num_Sidelobes; idx++)
              {
              sinc_val[idx] = float(sinc( idx + sinc_offset-Num_Sidelobes));
              }
            sinc_val[0] = float(0.6*sinc_val[0]);
            sinc_val[2*Num_Sidelobes-1] = float(0.6*sinc_val[2*Num_Sidelobes-1]);
            //---------------------------------------
            read_ptr = Read_Ptr_Start;

            //-----------------------------------------------
            sum = 0.0;
            zero_T = 0.0;
            for(int sum_idx=0; sum_idx< 2*Num_Sidelobes; sum_idx++)
            {
              sum += (*read_ptr) * sinc_val[sum_idx];
              read_ptr++;
              if(read_ptr > end_of_buffer)
              { 
                read_ptr = start_of_buffer;
                *DebugFile << "read_ptr wrapped" << endl;
              }
            }
          }
          Out_Samp_Count++;
          if(Out_Samp_Count == 2016)
          {
            iflag = 1;
          }
          *output_signal_ptr++ = sum;
          input_has_been_read = false;
        } // end of output generation
        else
        {
          // need to read some more inputs
          // are any avail?
          if(num_inputs_avail > 0)
          {
            input_samp = *input_signal_ptr++;
            *write_ptr++ = input_samp;
            if(write_ptr > end_of_buffer) 
            {
              write_ptr = start_of_buffer;
            }
            rt_brack_avail++;
            Read_Ptr_Start++;
            if(Read_Ptr_Start > end_of_buffer) Read_Ptr_Start = start_of_buffer;
          }
          num_inputs_avail--;
          input_has_been_read = true;
        }
      }

      output_block_size = Out_Samp_Count - beg_samp_count;
      Out_Sig->SetValidBlockSize(output_block_size);
      *DebugFile << "delayed output block size = " << output_block_size << endl;
      if(input_has_been_read)
      {
        *DebugFile << "input_has_been_read = true" << endl;
      }
      else
      {
        *DebugFile << "input_has_been_read = false" << endl;
      }
//      break;

  Rt_Brack_Avail = rt_brack_avail;
  Read_Ptr = read_ptr;
  Write_Ptr = write_ptr;
  return(_MES_AOK);
}
template RateChanger< int >;
template RateChanger< float >;
template RateChanger< std::complex<float> >;