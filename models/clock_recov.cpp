//
//  File = clock_recov.cpp
//

//#include <stdlib.h>
//#include <fstream.h>
#include "parmfile.h"
#include "misdefs.h"
#include "model_error.h"
#include "clock_recov.h"
#include "butt_filt_iir.h"
#include "model_graph.h"
//#include "sinc.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//======================================================

ClockRecoveryLoop::ClockRecoveryLoop( char* instance_name,
                        PracSimModel* outer_model,
                        Signal<float>* fsig_input,
                        Signal<float>* fsig_raw_error,
                        Signal<float>* fsig_filtered_error,
                        Signal<float>* fsig_symbol_period,
                        Signal<float>* out_sig )
                :PracSimModel(instance_name,
                              outer_model)
{
   MODEL_NAME(ClockRecoveryLoop);
   //ENABLE_MULTIRATE;

   //---------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;

   GET_DOUBLE_PARM(K_Sub_0);
   GET_DOUBLE_PARM(Center_Freq_Hz);
   GET_DOUBLE_PARM(Gate_Fraction);
   GET_DOUBLE_PARM(Nominal_Symbol_Period);
   Min_Symbol_Period = 0.96 * Nominal_Symbol_Period;
   Max_Symbol_Period = 1.04 * Nominal_Symbol_Period;
   //--------------------------------------
   //  Connect input and output signals

   fsig_Input = fsig_input;
   fsig_Filtered_Error = fsig_filtered_error;
   fsig_Raw_Error = fsig_raw_error;
   fsig_Symbol_Period = fsig_symbol_period;
   fsig_Output = out_sig;

   MAKE_OUTPUT( fsig_Output );
   MAKE_OUTPUT( fsig_Filtered_Error );
   MAKE_OUTPUT( fsig_Raw_Error );
   MAKE_OUTPUT( fsig_Symbol_Period );
   MAKE_INPUT( fsig_Input );

   //SAME_RATE(fsig_Input,fsig_Filtered_Error);
   //SAME_RATE(fsig_Input,fsig_Output);

   //----------------------------------
   //  Compute derived parameters

   //Omega_Sub_0 = TWO_PI * Center_Freq_Hz;
   Omega_Sub_0 = 1.0/Nominal_Symbol_Period;
   //--------------------------------
   // create loop filter object
   char sub_name[50];
   strcpy(sub_name, GetInstanceName());
   strcat(sub_name, ":Filter\0");
   Filter_Core = new ButterworthFilterByIir<float>( sub_name, this);
}

//======================================
ClockRecoveryLoop::~ClockRecoveryLoop( void ){ };

//=======================================
void ClockRecoveryLoop::Initialize(void)
{
   //------------------
   int block_size = fsig_Output->GetBlockSize();
   Samp_Intvl = fsig_Input->GetSampIntvl();
   Filter_Core->Initialize(block_size, Samp_Intvl);
   Reduced_Time = 0.0;
   Early_Gate_Open_Time = 0.0;
   Late_Gate_Close_Time = 0.0;
   Late_Gate_Is_Open = false;
   Early_Gate_Is_Open = true;
   Early_Gate_Sum = 0.0;
   Adjusted_Symbol_Period = Nominal_Symbol_Period;
   Gate_Diff = 0.0;
   Old_Input_Val = 0.0;
}
//=======================================================
int ClockRecoveryLoop::Execute()
{
   // pointers for signal data

   float *fsOutput_ptr;
   float *fs_filtered_error_ptr;
   float *fs_raw_error_ptr;
   float *fsInput_ptr;
   float *fs_symbol_period_ptr;

   float input_val;
//   float inst_freq;

   double samp_intvl;
   double filt_val=0;
   double err_sum=0;
   int is;
   int block_size;

   double time;
   double old_gate_diff=0;
   double closing_delay;
   double opening_delay;
   double val_at_close;
   double val_at_open;


   //--------------------------------------------------------------
   // set up pointers to data buffers for input and output signals

   fsOutput_ptr = GET_OUTPUT_PTR( fsig_Output );
   fs_filtered_error_ptr = GET_OUTPUT_PTR( fsig_Filtered_Error );
   fs_raw_error_ptr = GET_OUTPUT_PTR( fsig_Raw_Error );
   fs_symbol_period_ptr = GET_OUTPUT_PTR( fsig_Symbol_Period );

   fsInput_ptr = GET_INPUT_PTR( fsig_Input );
   //---------------------------------------------------------------

   samp_intvl = Samp_Intvl;

   block_size = fsig_Input->GetValidBlockSize();

   fsig_Output->SetValidBlockSize(block_size);
   fsig_Filtered_Error->SetValidBlockSize(block_size);
   fsig_Raw_Error->SetValidBlockSize(block_size);
   fsig_Symbol_Period->SetValidBlockSize(block_size);

   //--------------------------------------------------------------

   if(Reduced_Time > block_size * samp_intvl)
   {
      // reduce all the times by one pass duration
      Reduced_Time -= block_size * samp_intvl;
      Early_Gate_Open_Time -= block_size * samp_intvl;
      Early_Gate_Close_Time -= block_size * samp_intvl;
      Late_Gate_Open_Time -= block_size * samp_intvl;
      Late_Gate_Close_Time -= block_size * samp_intvl;
   }

   //-------------------------------------------------------

   for (is=0; is<block_size; is++)
   {
      time = Reduced_Time + is * samp_intvl;
      //*fsOscOutput_ptr++ = osc_output_val;
      input_val = *fsInput_ptr;
      if(is == 3100)
      {
         err_sum = 1;
      }

      //---------------------------------------------------
      // Late gate
      if(Late_Gate_Is_Open)
      {
         //late gate was open prior to current sample time

         if(time >= Late_Gate_Open_Time + Gate_Fraction * Adjusted_Symbol_Period)
         {
            // gate should be closed
            Late_Gate_Close_Time = Late_Gate_Open_Time + Gate_Fraction * Adjusted_Symbol_Period;
            Late_Gate_Is_Open = false;

            closing_delay = time - Late_Gate_Close_Time;
            val_at_close = input_val + closing_delay * (Old_Input_Val - input_val);
            Late_Gate_Latched_Sum = Late_Gate_Sum + (1.0 - closing_delay) * (val_at_close + Old_Input_Val)/2.0;
            Late_Gate_Sum = 0.0;
           // Gate_Diff = fabs(Early_Gate_Latched_Sum) - fabs(Late_Gate_Latched_Sum);
            Gate_Diff = fabs(Late_Gate_Latched_Sum) - fabs(Early_Gate_Latched_Sum);
      if(Gate_Diff != old_gate_diff)
      {
         old_gate_diff = Gate_Diff;
      }
      filt_val = Filter_Core->ProcessSample(Gate_Diff);
      Adjusted_Symbol_Period = Nominal_Symbol_Period - K_Sub_0 * filt_val;
//      if(Adjusted_Symbol_Period < Min_Symbol_Period) Adjusted_Symbol_Period = Min_Symbol_Period;
//      if(Adjusted_Symbol_Period > Max_Symbol_Period) Adjusted_Symbol_Period = Max_Symbol_Period;
         }
         else
         {
            // late gate remains open
            Late_Gate_Sum += input_val;
            Gate_Diff = 0.0;
         }
      }
      else
      {
         // late gate was closed prior to current sample time

         if(time >= Late_Gate_Close_Time + (1.0 - Gate_Fraction) * Adjusted_Symbol_Period)
         {
            // gate should be opened
            Late_Gate_Open_Time = Late_Gate_Close_Time + (1.0 - Gate_Fraction) * Adjusted_Symbol_Period;
            Late_Gate_Is_Open = true;

            opening_delay = time - Late_Gate_Open_Time;
            val_at_open = input_val + closing_delay * (Old_Input_Val - input_val);
            Late_Gate_Sum = opening_delay * (val_at_open + input_val)/2.0;
            //Late_Gate_Sum = input_val;
         }
         Gate_Diff = 0.0;
      }

      //---------------------------------------------------
      // Early gate
      if(Early_Gate_Is_Open)
      {
         //early gate was open prior to current sample time

         if(time >= Early_Gate_Open_Time + Gate_Fraction * Adjusted_Symbol_Period)
         {
            // gate should be closed
            Early_Gate_Close_Time = Early_Gate_Open_Time + Gate_Fraction * Adjusted_Symbol_Period;
            Early_Gate_Is_Open = false;

            closing_delay = time - Early_Gate_Close_Time;
            val_at_close = input_val + closing_delay * (Old_Input_Val - input_val);
            Early_Gate_Latched_Sum = Early_Gate_Sum + (1.0 - closing_delay) * (val_at_close + Old_Input_Val)/2.0;
            Early_Gate_Sum = 0.0;
         }
         else
         {
            // early gate remains open
            Early_Gate_Sum += input_val;
         }
      }
      else
      {
         // early gate was closed prior to current sample time

         if(time >= Early_Gate_Close_Time + (1.0 - Gate_Fraction) * Adjusted_Symbol_Period)
         {
            // gate should be opened
            Early_Gate_Open_Time = Early_Gate_Close_Time + (1.0 - Gate_Fraction) * Adjusted_Symbol_Period;
            Early_Gate_Is_Open = true;

            opening_delay = time - Early_Gate_Open_Time;
            val_at_open = input_val + closing_delay * (Old_Input_Val - input_val);
            Early_Gate_Sum = opening_delay * (val_at_open + input_val)/2.0;
            //Early_Gate_Sum = input_val;
         }
      }
      Old_Input_Val = input_val;
      //-------------------------------------
      //  filter the gate difference signal

//      if(Gate_Diff != old_gate_diff)
//      {
//         old_gate_diff = Gate_Diff;
//      }
//      filt_val = Filter_Core->ProcessSample(Gate_Diff);
      *fs_filtered_error_ptr++ = filt_val;
      *fs_raw_error_ptr++ = Gate_Diff;

      //----------------------------------------
      // use filtered error signal to drive VCO

      //inst_freq = Omega_Sub_0 + K_Sub_0 * filt_val;
      //Adjusted_Symbol_Period = 1.0/inst_freq;
      //Adjusted_Symbol_Period -= K_Sub_0 * filt_val;
      *fs_symbol_period_ptr++ = Adjusted_Symbol_Period;

      if(Early_Gate_Is_Open)
      {
         *fsOutput_ptr++ = 1.0;
      }
      else
      {
         *fsOutput_ptr++ = 0.0;
      }

      fsInput_ptr++;
   }
   Reduced_Time += block_size * samp_intvl;

  return(_MES_AOK);
}

