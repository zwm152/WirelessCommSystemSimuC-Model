//
//  File = decimate_sim.cpp
//

#define SIM_NAME "Decimate\0"
#define SIM_TITLE "Simple Decimation\0"

#include "global_stuff.h" 

#include "tone_gen.h"
#include "multi_tone_gen.h"
#include "andf1fir.h"
#include "cpxtophs.h"
#include "siganchr.h"
#include "bart_pdgm_wind.h"
#include "downsampler_T.h"
#include "discrete_delay_T.h"
#include "add_gaus_noise.h"
#include "spec_analyzer.h"
#include "pwrmtr.h"

//=========================================================

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(test_sig);
  FLOAT_SIGNAL(noisy_test_sig);
  FLOAT_SIGNAL(noise_only_sig);
  FLOAT_SIGNAL(filt_noise);
  FLOAT_SIGNAL(power_meas_sig);
  FLOAT_SIGNAL(delayed_test_sig);
  FLOAT_SIGNAL(downsamp_test_sig);
  FLOAT_SIGNAL(downsamp_noise);
  FLOAT_SIGNAL(filt_sig);
  FLOAT_SIGNAL(delayed_downsamp_sig);
  FLOAT_SIGNAL(est_avg_sig_pwr_1);
  FLOAT_SIGNAL(est_avg_sig_pwr_3);
  FLOAT_SIGNAL(est_avg_sig_pwr_4);
  FLOAT_SIGNAL(est_avg_sig_pwr_5);
  FLOAT_SIGNAL(est_avg_sig_pwr_6);
  

  //============================================================
  //  Construct, initialize and connect models

//   ToneGener* sig_gen = new  ToneGener( "sig_gen\0",
//                                       CommSystem,
//                                       test_sig );

   MultipleToneGener* sine_gen_1 = new MultipleToneGener( "sine_gen_1\0",
                       CommSystem,
                       test_sig );

   AdditiveGaussianNoise<float>* noise_gen = new AdditiveGaussianNoise<float> (
                                              "noise_gen\0",
                                              CommSystem,
                                              test_sig,
                                              noisy_test_sig,
                                              noise_only_sig,
                                              power_meas_sig);

   SpectrumAnalyzer<float>* spec_analyzer_1 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_1\0",
                                                CommSystem,
                                                test_sig );

   SpectrumAnalyzer<float>* spec_analyzer_7 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_7\0",
                                                CommSystem,
                                                noisy_test_sig );

   PowerMeter* power_meter_1 = new PowerMeter( "power_meter_1\0",
                                             CommSystem,
                                             test_sig,
                                             est_avg_sig_pwr_1);

   AnlgDirectFormFir* direct_fir = new AnlgDirectFormFir( "direct_fir\0",
                                                          CommSystem,
                                                          test_sig,
                                                          filt_sig );

   SpectrumAnalyzer<float>* spec_analyzer_5 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_5\0",
                                                CommSystem,
                                                filt_sig );

   PowerMeter* power_meter_5 = new PowerMeter( "power_meter_5\0",
                                             CommSystem,
                                             filt_sig,
                                             est_avg_sig_pwr_5);
   Downsampler< float > *downsampler = new Downsampler<float>( "downsampler\0",
                                CommSystem,
                                filt_sig,
                                downsamp_test_sig );

   SpectrumAnalyzer<float>* spec_analyzer_2 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_2\0",
                                                CommSystem,
                                                downsamp_test_sig );

   SpectrumAnalyzer<float>* spec_analyzer_3 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_3\0",
                                                CommSystem,
                                                noise_only_sig );

     PowerMeter* power_meter_3 = new PowerMeter( "power_meter_3\0",
                                             CommSystem,
                                             noise_only_sig,
                                             est_avg_sig_pwr_3);

 AnlgDirectFormFir* noise_fir = new AnlgDirectFormFir( "noise_fir\0",
                                                          CommSystem,
                                                          noise_only_sig,
                                                          filt_noise );

   SpectrumAnalyzer<float>* spec_analyzer_6 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_6\0",
                                                CommSystem,
                                                filt_noise );

   PowerMeter* power_meter_6 = new PowerMeter( "power_meter_6\0",
                                             CommSystem,
                                             filt_noise,
                                             est_avg_sig_pwr_6);

   Downsampler< float > *noise_downsampler = new Downsampler<float>( "noise_downsampler\0",
                                CommSystem,
                                filt_noise,
                                downsamp_noise );

   SpectrumAnalyzer<float>* spec_analyzer_4 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_4\0",
                                                CommSystem,
                                                downsamp_noise );

   PowerMeter* power_meter_4 = new PowerMeter( "power_meter_4\0",
                                             CommSystem,
                                             downsamp_noise,
                                             est_avg_sig_pwr_4);

   SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig);
  //=============================================================
  #include "sim_postamble.cpp"

  return 0;
}  
