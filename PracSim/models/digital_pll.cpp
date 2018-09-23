//
//  File = digital_pll.cpp
//

#include "parmfile.h"
#include "misdefs.h"
#include "model_error.h"
#include "digital_pll.h"
#include "butt_filt_iir.h"
#include "model_graph.h"
extern ParmFile *ParmInput;
extern PracSimModel *ActiveModel;

//======================================================

DigitalPLL::DigitalPLL( 
                    char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* fsig_input,
                    Signal<float>* fsig_filtered_error,
                    Signal<float>* fsig_vco_output,
                    Signal<float>* vco_freq_sig,
                    Signal<float>* ref_phase_sig,
                    Signal<float>* out_sig )
              :PracSimModel( instance_name,
                             outer_model)
{
   MODEL_NAME(DigitalPLL);

   //---------------------------------------
   //  Read model config parms

   OPEN_PARM_BLOCK;

   GET_DOUBLE_PARM(K_Sub_D);
   GET_DOUBLE_PARM(K_Sub_0);
   GET_DOUBLE_PARM(Center_Freq_Hz);
   GET_BOOL_PARM(m_UsingDco);
   GET_DOUBLE_PARM(Tau_1);
   GET_DOUBLE_PARM(Tau_2);
   GET_DOUBLE_PARM(Supply_Volts);

   //--------------------------------------
   //  Connect input and output signals

   fsig_Input = fsig_input;
   fsig_Filtered_Error = fsig_filtered_error;
   fsig_Osc_Output = fsig_vco_output;
   fsig_Osc_Freq = vco_freq_sig;
   fsig_Osc_Phase = ref_phase_sig;
   fsig_Output = out_sig;

   MAKE_OUTPUT( fsig_Output );
   MAKE_OUTPUT( fsig_Filtered_Error );
   MAKE_OUTPUT( fsig_Osc_Output );
   MAKE_OUTPUT( fsig_Osc_Freq );
   MAKE_OUTPUT( fsig_Osc_Phase );
   MAKE_INPUT( fsig_Input );

   //----------------------------------
   //  Compute derived parameters

   Omega_Sub_0 = TWO_PI * Center_Freq_Hz;
   //--------------------------------
   // create loop filter object
   char sub_name[50];
   strcpy(sub_name, GetInstanceName());
   strcat(sub_name, ":Filter\0");
   Filter_Core = new 
      ButterworthFilterByIir<float>( sub_name, this);
}

//======================================================
DigitalPLL::~DigitalPLL( void ){ };

//======================================================
void DigitalPLL::Initialize(void)
{
   //------------------
   int block_size = fsig_Output->GetBlockSize();
   Samp_Intvl = fsig_Input->GetSampIntvl();
   Filter_Core->Initialize(block_size, Samp_Intvl);
   Osc_Output_Prev_Val = 0.0;
   OscOutput = 0;
   Phi_Sub_2 = 0;
   Prev_Input_Positive = true;

   Prev_Input_Val = 0;
   Prev_Filt_Val = 0;
   Prev_Time_Zc = 0;
   Prev_State = 0;
   Prev_Cap_Val = 0;
   Time_Of_Samp = 0.0;
   Prev_Osc_Phase = 0.0;

}
//======================================================
int DigitalPLL::Execute()
{
   // pointers for signal data

   float *fsOutput_ptr;
   float *fs_filtered_error_ptr;
   float *fsOscOutput_ptr;
   float *fsOscFreq_ptr;
   float *fsOscPhase_ptr;
   float *fsInput_ptr;

   float input_val;
   float prev_input_val;
   float osc_output_val;
   float filt_val;
   float inst_freq;

   double samp_intvl;
   double err_sum=0;
   double err_avg;
   int is;
   int block_size;
   double time_of_samp;
   double time_zc;
   double delta_T;
   double osc_phase;
   double cap_val;
   double prev_cap_val;
   double prev_osc_phase;
   double prev_filt_val;
   double prev_time_zc;
   double output_phase;
   double tau_n;
   int prev_state;
   int new_state;
   double time_dwell_plus;
   double time_dwell_minus;


   //---------------------------------------------------
   // set up pointers to data buffers for input and 
   // output signals

   fsOutput_ptr = GET_OUTPUT_PTR( fsig_Output );
   fs_filtered_error_ptr = 
               GET_OUTPUT_PTR( fsig_Filtered_Error );

   fsOscOutput_ptr = GET_OUTPUT_PTR( fsig_Osc_Output );
   fsOscFreq_ptr = GET_OUTPUT_PTR( fsig_Osc_Freq );
   fsOscPhase_ptr = GET_OUTPUT_PTR( fsig_Osc_Phase );
   fsInput_ptr = GET_INPUT_PTR( fsig_Input );

   samp_intvl = Samp_Intvl;
   osc_output_val = Osc_Output_Prev_Val;
   prev_input_val = Prev_Input_Val;
   prev_filt_val = Prev_Filt_Val;
   prev_time_zc = Prev_Time_Zc;
   prev_state = Prev_State;
   prev_cap_val = Prev_Cap_Val;
   prev_osc_phase = Prev_Osc_Phase;

   block_size = fsig_Input->GetValidBlockSize();

   fsig_Output->SetValidBlockSize(block_size);
   fsig_Filtered_Error->SetValidBlockSize(block_size);
   fsig_Osc_Output->SetValidBlockSize(block_size);
   fsig_Osc_Freq->SetValidBlockSize(block_size);
   fsig_Osc_Phase->SetValidBlockSize(block_size);

   for (is=0; is<block_size; is++){
      time_of_samp = Time_Of_Samp + (is+1)*samp_intvl;
      //----------------------------------
      //  Look for zero crossing 
      *fsOscOutput_ptr++ = osc_output_val;
      input_val = *fsInput_ptr;

      if( (input_val >= 0) != Prev_Input_Positive){
         // zero crossing has occurred
         time_zc = time_of_samp - samp_intvl * 
               input_val /(input_val - prev_input_val);

         // compute elapsed interval
         delta_T = time_zc - prev_time_zc;

         // update oscillator phase
         inst_freq = 
               Omega_Sub_0 + K_Sub_0 * prev_filt_val;
         osc_phase = 
               prev_osc_phase + inst_freq * delta_T;

         // based on osc_phase and prev_osc_phase, 
         // determine if the oscillator waveform has
         // had a positive-going zero crossing between
         // times prev_time_zc and time_zc.
         //
         // Normalize osc_phase and prev-osc_phase
         if(osc_phase > TWO_PI){
            osc_phase -= TWO_PI;
            prev_osc_phase -= TWO_PI;
         }

         if(osc_phase >= 0.0 && prev_osc_phase < 0.0){
            // a positive-going zero crossing has 
            // occurred, so compute the crossing time
            tau_n = -delta_T * prev_osc_phase/ 
                        ( osc_phase - prev_osc_phase);
         }
         else{
            tau_n = 0.0;
         }
         //------------------------------------------
         // do state machine for phase detector
         switch (prev_state){
         case 1:
            if(tau_n !=0.0){
               new_state = 0;
               time_dwell_plus = tau_n;
               time_dwell_minus = 0.0;
            }
            else{
               new_state = 0;
               time_dwell_plus = delta_T;
               time_dwell_minus = 0.0;
            }
            break;
         case -1:
            if(!Prev_Input_Positive){
               //step 3 Algorithm 12.3
               new_state = -1;
               time_dwell_plus = 0.0;
               time_dwell_minus = delta_T;
            }
            else{
               if(tau_n != 0.0){
                  //step 4a Algorithm 12.3
                  new_state = -1;
                  time_dwell_plus = 0.0;
                  time_dwell_minus = delta_T - tau_n;
               }
               else{
                   //step 4b Algorithm 12.3
                  new_state = 0;
                  time_dwell_plus = 0.0;
                  time_dwell_minus = 0.0;
              }
            }
            break;
         case 0:
            if(Prev_Input_Positive){
               if(tau_n != 0.0 ){
                  // step 5a Algorithm 12.3
                  new_state = 0;
                  time_dwell_plus = tau_n;
                  time_dwell_minus = 0.0;
               }
               else{
                  // step 5b Algorithm 12.3
                  new_state = 1;
                  time_dwell_plus = delta_T;
                  time_dwell_minus = 0.0;
               }
            }
            else{
               if(tau_n != 0.0 ){
                  // step 6a Algorithm 12.3
                  new_state = -1;
                  time_dwell_plus = 0.0;
                  time_dwell_minus = delta_T - tau_n;
               }
               else{
                  // step 6b Algorithm 12.3
                  new_state = 0;
                  time_dwell_plus = 0.0;
                  time_dwell_minus = 0.0;
               }
            }
         }
         // end of state machine
         //------------------------------------------
         // Perform Filtering
         //
         if(time_dwell_plus == 0 && 
                              time_dwell_minus == 0){
            // step 6 Algorithm 12.5
            cap_val = prev_cap_val;
            filt_val = cap_val;
         }
         else{
            if(time_dwell_minus > 0){
               // step 7 Algorithm 12.5
               cap_val = prev_cap_val*(1.0 - 
                     time_dwell_minus/(Tau_1 + Tau_2));
               filt_val = cap_val * (1.0 - (Tau_2/
                     (Tau_1+Tau_2))*
                     (time_dwell_minus/delta_T));
            }
            else{
               // step 8 Algorithm 12.5
               cap_val = prev_cap_val + 
                     (time_dwell_plus/(Tau_1 + Tau_2))*
                     (Supply_Volts - prev_cap_val);
               filt_val = cap_val + 
                     (time_dwell_plus/delta_T)* 
                     (Tau_2/(Tau_1+Tau_2))* 
                     (Supply_Volts - cap_val);
            }
         }
         //---------------------------------------------
         // update delayed variables
         prev_state = new_state;
         prev_time_zc = time_zc;
         prev_osc_phase = osc_phase;
         prev_cap_val = cap_val;
         prev_filt_val = filt_val;
         Prev_Input_Positive = (input_val >= 0);
      }

      delta_T = time_of_samp - prev_time_zc;
      inst_freq = Omega_Sub_0 + K_Sub_0 * prev_filt_val;
      output_phase = 
                  prev_osc_phase + inst_freq * delta_T;
      *fsOutput_ptr++ = sin(output_phase);
      *fs_filtered_error_ptr++ = prev_filt_val;
      *fsOscPhase_ptr++ = output_phase;
      *fsOscFreq_ptr++ = inst_freq/TWO_PI;
      fsInput_ptr++;
   }
   Prev_Input_Val = prev_input_val;
   Prev_Filt_Val = prev_filt_val;
   Prev_Time_Zc = prev_time_zc;
   Prev_State = prev_state;
   Prev_Cap_Val = prev_cap_val;
   Time_Of_Samp = time_of_samp;
   Prev_Osc_Phase = prev_osc_phase;


   err_avg = err_sum / block_size;
   BasicResults << "avg PLL error = " << err_avg 
                << endl;
   Osc_Output_Prev_Val = osc_output_val;

  return(_MES_AOK);
}

