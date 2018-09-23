//
//  File = multitonegentest_sim.cpp
//

#define SIM_NAME "MultiToneGenTest\0"
#define SIM_TITLE "Multiple Tone Generator Test\0"

#include "global_stuff.h" 

#include "multi_tone_gen.h"
#include "siganchr.h"
#include "spec_analyzer.h"

//=========================================================

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(test_sig);

  //============================================================
  //  Construct, initialize and connect models


   MultipleToneGener* multi_tone_gen = new MultipleToneGener( "multi_tone_gen\0",
                       CommSystem,
                       test_sig );

   SpectrumAnalyzer<float>* spec_analyzer_1 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_1\0",
                                                CommSystem,
                                                test_sig );


   SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig);
  //=============================================================
  #include "sim_postamble.cpp"

  return 0;
}  
