//
//  File = gaussnoisetest_sim.cpp
//

#define SIM_NAME "GaussNoiseTest\0"
#define SIM_TITLE "Gaussian Noise Testbed\0"

#include "global_stuff.h" 

#include "gauss_theory.h"
#include "level_gen.h"
#include "histogram.h"
#include "ogive.h"
#include "siganchr.h"
#include "add_gaus_noise.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

   GaussPdf(   0.0,//mean
               0.5,//standard deviation
               401,// num pts
               50, // pts per s.d.
               "gauss_pdf.txt\0");

   GaussCdf(   0.0,//mean
               0.5002249,//standard deviation
               400,// num pts
               50, // pts per s.d.
               "gauss_cdf.txt\0");

  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(flat_line_sig);
  FLOAT_SIGNAL(noisy_sig);
  FLOAT_SIGNAL(power_meas_sig);

  //============================================================
  //  Construct, initialize and connect models

  
   LevelGener* lev_gen = new LevelGener(  "lev_gen\0",
                                          CommSystem,
                                          flat_line_sig );

   SignalAnchor* flat_line_anchr = new SignalAnchor( "flat_line_anchr\0",
                                                   CommSystem,
                                                   flat_line_sig );

   AdditiveGaussianNoise<float>* awgn_source = new AdditiveGaussianNoise<float>( 
                                                "awgn_source\0",
                                                CommSystem,
                                                flat_line_sig,
                                                noisy_sig,
                                                power_meas_sig);

//   DiscreteAutoCovar* disc_autocovar = new DiscreteAutoCovar( "disc_autocovar\0",
//                                CommSystem,
//                                bit_seq);
   HistogramBuilder<float>* histogram = new HistogramBuilder<float>(   "histogram\0",
                                                         CommSystem,
                                                         noisy_sig);

   OgiveBuilder<float>* ogive = new OgiveBuilder<float>(   "ogive\0",
                                                         CommSystem,
                                                         noisy_sig);


  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
