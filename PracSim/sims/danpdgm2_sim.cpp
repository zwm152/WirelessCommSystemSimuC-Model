//
//  File = danpdgm2_sim.cpp
//

#define SIM_NAME "DanPdgm2\0"
#define SIM_TITLE "Daniell Periodogram Testbed\0"

#include "global_stuff.h" 

#include "siganchr.h"
#include "sin_awgn.h"
#include "dan_pdgm.h"
#include "ar_spec.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing
   double a_coeffs[3];
   a_coeffs[0] = 1.0;
   a_coeffs[1] = -0.65;
   a_coeffs[2] = 0.75;
   ArSpectrum *ar_spectrum = new ArSpectrum(  2,//true_ar_order,
                                          a_coeffs,
                                          0.125,//0.0009765625, //samp interval
                                          0.3, //true_ar_drv_var );
                                          512,
                                          0.5);
   ar_spectrum->DumpSpectrum( "ar_true_spec.txt\0",
                              true);


  //=========================================================================
  //  Allocate signals

  FLOAT_SIGNAL(test_sig);

  //============================================================
  //  Construct, initialize and connect models

  
   SinesInAwgn* sig_source = new SinesInAwgn(   "sig_source\0",
                                                CommSystem,
                                                 test_sig);


   SignalAnchor* sig_anchr = new SignalAnchor(  "sig_anchr\0",
                                                CommSystem,
                                                test_sig );


   DaniellPeriodogram<float>* spec_estimator = 
                  new DaniellPeriodogram<float>(  "spec_estimator\0",
                                                CommSystem,
                                                test_sig );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
