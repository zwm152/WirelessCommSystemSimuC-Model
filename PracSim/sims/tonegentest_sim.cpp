//
//  File = tonegentest_sim.cpp
//

#define SIM_NAME "ToneGenTest\0"
#define SIM_TITLE "Tone Generator Test\0"

#include "global_stuff.h" 

#include "tone_gen.h"
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
  FLOAT_SIGNAL(ref_sig);
  FLOAT_SIGNAL(delayed_ref_sig);
  FLOAT_SIGNAL(upsamp_test_sig);
  FLOAT_SIGNAL(filt_sig);
  FLOAT_SIGNAL(correl_out_sig);
  FLOAT_SIGNAL(interm_filt_sig_1);
  FLOAT_SIGNAL(interm_filt_sig_2);
  FLOAT_SIGNAL(delayed_filt_sig);

  //============================================================
  //  Construct, initialize and connect models

   ToneGener* tone_gen = new  ToneGener( "tone_gen\0",
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
