//
//  File = decimate_sim.cpp
//

#define SIM_NAME "Zm_Nonlin\0"
#define SIM_TITLE "Memoryless Nonlinearities\0"

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
#include "ideal_am.h"
#include "ideal_hard_lim.h"

//=========================================================

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(test_sig);
  FLOAT_SIGNAL(limited_sig);
  FLOAT_SIGNAL(carrier_sig);
  FLOAT_SIGNAL(noisy_test_sig);
  FLOAT_SIGNAL(noise_only_sig);
  FLOAT_SIGNAL(lp_noise_sig);
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
                       carrier_sig );

   AdditiveGaussianNoise<float>* noise_gen = new AdditiveGaussianNoise<float> (
                                              "noise_gen\0",
                                              CommSystem,
                                              carrier_sig,
                                              noisy_test_sig,
                                              noise_only_sig,
                                              power_meas_sig);

   AnlgDirectFormFir* noise_filt = new AnlgDirectFormFir( "noise_filt\0",
                                                          CommSystem,
                                                          noise_only_sig,
                                                          lp_noise_sig );


   IdealAmplitudeModulator* modulator = new IdealAmplitudeModulator( "modulator\0",
                                                                  CommSystem,
                                                                  carrier_sig,
                                                                  lp_noise_sig,
                                                                  test_sig);

   SpectrumAnalyzer<float>* spec_analyzer_1 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_1\0",
                                                CommSystem,
                                                test_sig );

    IdealHardLimiter* limiter = new IdealHardLimiter( "limiter\0",
                                                      CommSystem,
                                                      test_sig,
                                                      limited_sig);

   SpectrumAnalyzer<float>* spec_analyzer_2 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_2\0",
                                                CommSystem,
                                                limited_sig );

  AnlgDirectFormFir* direct_fir = new AnlgDirectFormFir( "direct_fir\0",
                                                          CommSystem,
                                                          limited_sig,
                                                          filt_sig );

   SpectrumAnalyzer<float>* spec_analyzer_3 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_3\0",
                                                CommSystem,
                                                filt_sig );


   SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig);
  //=============================================================
  #include "sim_postamble.cpp"

  return 0;
}  
