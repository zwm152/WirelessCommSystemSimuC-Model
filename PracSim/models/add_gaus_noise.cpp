//
//  File = add_gaus_noise.cpp
//

#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "parmfile.h"
#include "add_gaus_noise.h"
#include "syst_graph.h"
#include "misdefs.h"
#include "gensig.h"
#include "butt_filt_iir.h"
#include "gausrand.h"
extern int PassNumber;
extern ParmFile *ParmInput;
extern SystemGraph CommSystemGraph;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
// normal constructor

template <class T>
AdditiveGaussianNoise<T>
         ::AdditiveGaussianNoise( 
                          char* instance_name,
                          PracSimModel* outer_model,
                          Signal<T>* in_sig,
                          Signal<T>* noisy_sig,
                          Signal<float>* power_meas_sig)
                 :PracSimModel(  instance_name,
                                 outer_model)
{
   MODEL_NAME(AdditiveGaussianNoise);
   char sub_name[60];
   In_Sig = in_sig;
   Noisy_Sig = noisy_sig;
   Power_Meas_Sig = power_meas_sig;

   OPEN_PARM_BLOCK;

   GET_FLOAT_PARM(Anticip_Input_Pwr);
   GET_FLOAT_PARM(Desired_Output_Pwr);
   GET_FLOAT_PARM(Desired_Eb_No);
   GET_FLOAT_PARM(Symb_Period);
   GET_FLOAT_PARM(Num_Bits_Per_Symb);
   GET_FLOAT_PARM(Time_Const_For_Pwr_Mtr);
   GET_INT_PARM(Seed);
   GET_BOOL_PARM(Sig_Pwr_Meas_Enabled);
   GET_BOOL_PARM(Outpt_Pwr_Scaling_On);

   strcpy(sub_name,GetModelName());
   strcat(sub_name, ":k_PowerMeter\0");
   Power_Meter = 
      new k_PowerMeter<T>( sub_name,
                           Anticip_Input_Pwr,
                           Time_Const_For_Pwr_Mtr);

   MAKE_INPUT(In_Sig);
   MAKE_OUTPUT(Noisy_Sig);
   MAKE_OUTPUT(Power_Meas_Sig);
   Noise_Only_Sig = NULL;

}
//======================================================
// constructor that connects a noise-only output signal

template <class T>
AdditiveGaussianNoise<T>::AdditiveGaussianNoise( 
                          char* instance_name,
                          PracSimModel* outer_model,
                          Signal<T>* in_sig,
                          Signal<T>* noisy_sig,
                          Signal<T>* noise_only_sig,
                          Signal<float>* power_meas_sig)
                      :PracSimModel(instance_name,
                                    outer_model)
{
   MODEL_NAME(AdditiveGaussianNoise);
   char sub_name[60];
   In_Sig = in_sig;
   Noisy_Sig = noisy_sig;
   Power_Meas_Sig = power_meas_sig;
   Noise_Only_Sig = noise_only_sig;

   OPEN_PARM_BLOCK;

   GET_FLOAT_PARM(Anticip_Input_Pwr);
   GET_FLOAT_PARM(Desired_Output_Pwr);
   GET_FLOAT_PARM(Desired_Eb_No);
   GET_FLOAT_PARM(Symb_Period);
   GET_FLOAT_PARM(Num_Bits_Per_Symb);
   GET_FLOAT_PARM(Time_Const_For_Pwr_Mtr);
   GET_INT_PARM(Seed);
   GET_BOOL_PARM(Sig_Pwr_Meas_Enabled);
   GET_BOOL_PARM(Outpt_Pwr_Scaling_On);

   strcpy(sub_name,GetModelName());
   strcat(sub_name, ":k_PowerMeter\0");
   Power_Meter = new k_PowerMeter<T>(  
                           sub_name,
                           Anticip_Input_Pwr,
                           Time_Const_For_Pwr_Mtr);
   MAKE_INPUT(In_Sig);
   MAKE_OUTPUT(Noisy_Sig);
   MAKE_OUTPUT(Power_Meas_Sig);
   MAKE_OUTPUT(Noise_Only_Sig);

}

//=======================================================
// destructor

template <class T>
AdditiveGaussianNoise<T>::~AdditiveGaussianNoise( void )
{
    delete Power_Meter;
};
//======================================================
template < class T>
void AdditiveGaussianNoise<T>::Initialize(void)
{
   double bit_weight, ebno_scaled;

   *DebugFile 
      << "Now in AdditiveGaussianNoise::Initialize()" 
      << endl;

   Proc_Block_Size = In_Sig->GetBlockSize();
   ebno_scaled = double( pow(10.0,(Desired_Eb_No/10.))*
      (In_Sig->GetSampIntvl()) );

   bit_weight = Symb_Period / Num_Bits_Per_Symb;
   Noise_Sigma = float(sqrt( (Anticip_Input_Pwr * 
                     bit_weight) / ebno_scaled / 2.0 ));
   Power_Scaler = float(bit_weight / ebno_scaled);

   Sum = 0.0;
   Sum_Sqrd = 0.0;
   Num_Samps = 0;
   Cumul_Batch_Power = 0.0;

   Power_Meter->Initialize( Proc_Block_Size, 
                            In_Sig->GetSampIntvl());
}
//======================================================
template <class T>
int AdditiveGaussianNoise<T>::Execute(void)
{
   int is;
   T *in_sig, *noisy_sig;
   T *noise_only_sig;
   float *sig_pwr_sig, sig_pwr, noise_sigma;
   std::complex<float> cmpx_in, cmpx_rand_var;
   T rand_var;
   T noisy_sig_val;
   T noise_val;

   float anticip_input_pwr = Anticip_Input_Pwr;
   float desired_output_pwr = Desired_Output_Pwr;
   float power_scaler = Power_Scaler;
   long seed = Seed;

   Proc_Block_Size = In_Sig->GetValidBlockSize();
   Noisy_Sig->SetValidBlockSize(Proc_Block_Size);
   Power_Meas_Sig->SetValidBlockSize(Proc_Block_Size);
   if(Noise_Only_Sig != NULL) 
     Noise_Only_Sig->SetValidBlockSize(Proc_Block_Size);

   //---------------------------------------------------
   // determine the power of the input signal

   in_sig = GET_INPUT_PTR(In_Sig);
   double sum = 0.0;
   for(is=0; is<Proc_Block_Size; is++){
      cmpx_in = *in_sig;
      sum += std::norm(cmpx_in);
      in_sig++;
   }
   Cumul_Batch_Power += sum/double(Proc_Block_Size);
   if( (PassNumber % 10) == 0) {
      BasicResults << PassNumber << " Batch power = " 
         << float(sum/double(Proc_Block_Size)) 
         << "  Cumul = " 
         << float(Cumul_Batch_Power/double(PassNumber))
         << endl;
   }
   in_sig = GET_OUTPUT_PTR(In_Sig);
   sig_pwr_sig = GET_OUTPUT_PTR(Power_Meas_Sig);
   Power_Meter->Execute(in_sig,sig_pwr_sig,
                        Proc_Block_Size);

   // this only needed when signal power is not measured
   sig_pwr = Anticip_Input_Pwr;

   noise_sigma = 
      float(sqrt(sig_pwr * power_scaler / 2.0));

   in_sig = GET_INPUT_PTR(In_Sig);
   noisy_sig = GET_OUTPUT_PTR(Noisy_Sig);
   if(Noise_Only_Sig != NULL) {
      noise_only_sig = GET_OUTPUT_PTR(Noise_Only_Sig);
   }

   //---------------------------------------------------
   //  main loop
   sig_pwr_sig = GET_OUTPUT_PTR(Power_Meas_Sig);

   if( (PassNumber % 50) == 0) {
      BasicResults << "   Metered power = " 
         << float(*(sig_pwr_sig + Proc_Block_Size - 1))
         << endl;
   }

   for(is=0; is<Proc_Block_Size; is++){
      if(Sig_Pwr_Meas_Enabled){
         sig_pwr = *sig_pwr_sig++;

         noise_sigma = 
            float(sqrt( sig_pwr * power_scaler / 2.0 ));
      }

      // generate gaussian RV
      GaussRandom(&seed, &rand_var);
      Sum += rand_var;
      cmpx_rand_var = rand_var;
      Sum_Sqrd += std::norm(cmpx_rand_var);
      Num_Samps ++;

      // combine noise with signal
      noise_val = noise_sigma * rand_var;
      noisy_sig_val = *in_sig + noise_val;
      in_sig++;

      // if specified, apply scaling to output
      if( (Outpt_Pwr_Scaling_On) && 
         (desired_output_pwr > 0.0)){
         noisy_sig_val *= 
            float(sqrt(desired_output_pwr/sig_pwr));
         noise_val *= 
            float(sqrt(desired_output_pwr/sig_pwr));
      }
      *noisy_sig++ = noisy_sig_val;
      if(Noise_Only_Sig != NULL) {
         *noise_only_sig++ = noise_val;
      }
   }// end of main loop

   // put back variables that have changed
   Seed = seed;

   if( PassNumber%100 == 0){
      complex<double> avg = Sum / double(Num_Samps);
      double var = 
         (Sum_Sqrd / Num_Samps) - std::norm(avg);
      BasicResults << "Pass " << PassNumber << " avg = "
                   << avg << "  var = " << var << endl;
      BasicResults << "noise_sigma = " << noise_sigma 
                   << endl;
   }
   return(_MES_AOK);
};
template AdditiveGaussianNoise< complex<float> >;
template AdditiveGaussianNoise<float>;

