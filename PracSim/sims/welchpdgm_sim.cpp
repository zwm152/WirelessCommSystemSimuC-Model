//
//  File = bartpdgm_sim.cpp
//

#define SIM_NAME "WelchPdgm\0"
#define SIM_TITLE "Welch Periodogram Testbed\0"

#include "global_stuff.h" 

#include "siganchr.h"
#include "sines_in_awgn.h"
#include "welch_pdgm.h"
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
                                          0.0625,//0.0009765625, //samp interval
                                          0.32, //true_ar_drv_var );
                                          512,
                                          0.5);
   ar_spectrum->DumpSpectrum( "ar_true_spec.txt\0",
                              true);


  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(test_sig);

  //============================================================
  //  Construct, initialize and connect models

  
   SinesInAwgn* sig_source = new SinesInAwgn( "sig_source\0",
                                                CommSystem,
                                                test_sig );

   SignalAnchor* sig_anchr = new SignalAnchor(  "sig_anchr\0",
                                                CommSystem,
                                                test_sig );


   WelchPeriodogram<float>* spec_analyzer = 
                  new WelchPeriodogram<float>(  "spec_estimator\0",
                                                CommSystem,
                                                test_sig );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
