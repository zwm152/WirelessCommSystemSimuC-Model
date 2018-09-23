//
//  File = arproctest_sim.cpp
//

#define SIM_NAME "ArProcTest\0"
#define SIM_TITLE "Autoregressive Noise Testbed\0"

#include "global_stuff.h" 

#include "rayleigh_theory.h"
#include "level_gen.h"
#include "disc_auto_cov.h"
#include "histogram.h"
#include "ogive.h"
#include "siganchr.h"
#include "ar_proc_gen.h"
#include "spec_analyzer.h"
#include "ar_spec.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing
   double a_coeffs[3];
   a_coeffs[0] = 1.0;
   a_coeffs[1] = -0.6;
   a_coeffs[2] = 0.8;
   ArSpectrum *ar_spectrum = new ArSpectrum(  2,//true_ar_order,
                                          a_coeffs,
                                          0.0009765625,
                                          0.32,
                                          512,
                                          0.5);//true_ar_drv_var );
   ar_spectrum->DumpSpectrum( "ar_true_spec.txt\0",
                              true);

  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(noisy_sig);

  //============================================================
  //  Construct, initialize and connect models

  
   ArProcessGenerator<float>* noise_source = new ArProcessGenerator<float>( 
                                                         "noise_source\0",
                                                         CommSystem,
                                                         noisy_sig);

   SignalAnchor* sig_anchr = new SignalAnchor(  "sig_anchr\0",
                                                CommSystem,
                                                noisy_sig );

//   DiscreteAutoCovar* disc_autocovar = new DiscreteAutoCovar( "disc_autocovar\0",
//                                CommSystem,
//                                bit_seq);
   HistogramBuilder<float>* histogram = new HistogramBuilder<float>(   "histogram\0",
                                                         CommSystem,
                                                         noisy_sig);

//   OgiveBuilder<float>* ogive = new OgiveBuilder<float>(   "ogive\0",
//                                                         CommSystem,
//                                                         noisy_sig);

   SpectrumAnalyzer<float>* spec_analyzer = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer\0",
                                                CommSystem,
                                                noisy_sig );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
