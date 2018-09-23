//
//  File = rayleighnoisetest_sim.cpp
//

#define SIM_NAME "RayleighNoiseTest\0"
#define SIM_TITLE "Rayleigh Noise Testbed\0"

#include "global_stuff.h" 

#include "rayleigh_theory.h"
#include "level_gen.h"
#include "histogram.h"
#include "ogive.h"
#include "siganchr.h"
#include "rayl_gen.h"
//#include "spec_analyzer.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

   RayleighPdf(   1.0,//standard deviation
               401,// num pts
               50, // pts per s.d.
               "rayl_pdf.txt\0");

   RayleighCdf(   1.0,//standard deviation
               400,// num pts
               50, // pts per s.d.
               "rayl_cdf.txt\0");

  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(flat_line_sig);
  FLOAT_SIGNAL(noisy_sig);
  FLOAT_SIGNAL(power_meas_sig);

  //============================================================
  //  Construct, initialize and connect models

  
   RayleighNoiseGenerator<float>* noise_source = new RayleighNoiseGenerator<float>( 
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

   OgiveBuilder<float>* ogive = new OgiveBuilder<float>(   "ogive\0",
                                                         CommSystem,
                                                         noisy_sig);

//   SpectrumAnalyzer<float>* spec_analyzer = 
//                  new SpectrumAnalyzer<float>(  "spec_analyzer\0",
//                                                CommSystem,
//                                                noisy_sig );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
