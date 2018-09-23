//
//  File = interpolate_sim.cpp
//

#define SIM_NAME "Interpolate\0"
#define SIM_TITLE "Simple Interpolation\0"

#include "global_stuff.h" 

#include "tone_gen.h"
#include "multi_tone_gen.h"
//#include "cpx_freq_dom_filt.h"
//#include "cpxtoquad.h"
//#include "quad_to_polar.h"
#include "andf1fir.h"
#include "cpxtophs.h"
#include "siganchr.h"
//#include "bart_pdgm_wind.h"
#include "ms_error_T.h"
#include "upsampler_T.h"
#include "discrete_delay_T.h"
#include "spec_analyzer.h"
#include "correlator.h"
#include "contin_delay2_T.h"

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

//   ToneGener* sig_gen = new  ToneGener( "sig_gen\0",
//                                       CommSystem,
//                                       test_sig );


   MultipleToneGener* sine_gen_1 = new MultipleToneGener( "sine_gen_1\0",
                       CommSystem,
                       test_sig );

   MultipleToneGener* ref_sine_gen = new MultipleToneGener( "ref_sine_gen\0",
                       CommSystem,
                       ref_sig );

   SpectrumAnalyzer<float>* spec_analyzer_1 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_1\0",
                                                CommSystem,
                                                ref_sig );

   Upsampler< float > *upsampler = new Upsampler<float>( "upsampler\0",
                                CommSystem,
                                test_sig,
                                upsamp_test_sig );

   SpectrumAnalyzer<float>* spec_analyzer_2 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_2\0",
                                                CommSystem,
                                                upsamp_test_sig );

//   SincInterpolator* recon_box = 
//                  new SincInterpolator(   "recon_box\0",
//                                          CommSystem,
//                                          tone_sig);

   AnlgDirectFormFir* direct_fir = new AnlgDirectFormFir( "direct_fir\0",
                                                          CommSystem,
                                                          upsamp_test_sig,
                                                          interm_filt_sig_1 );

   AnlgDirectFormFir* direct_fir_2 = new AnlgDirectFormFir( "direct_fir_2\0",
                                                          CommSystem,
                                                          interm_filt_sig_1,
                                                          interm_filt_sig_2 );

   AnlgDirectFormFir* direct_fir_3 = new AnlgDirectFormFir( "direct_fir_3\0",
                                                          CommSystem,
                                                          interm_filt_sig_2,
                                                          filt_sig );

   SpectrumAnalyzer<float>* spec_analyzer_3 = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer_3\0",
                                                CommSystem,
                                                filt_sig );

  Control<float>* delay_at_max_corr = new Control<float>( "delay_at_max_corr\0",
                                                   CommSystem );

  Control<int>* samps_delay_at_max_corr = new Control<int>( "samps_delay_at_max_corr\0",
                                                   CommSystem );

//   RealCorrelator *ref_correl = new RealCorrelator( "ref_correl\0",
//                                                      CommSystem,
//                                                      filt_sig,
//                                                      ref_sig,
//                                                      correl_out_sig,
//                                                      delay_at_max_corr,
//                                                      samps_delay_at_max_corr);

   ContinuousDelay2< float >* cont_ref_delay = new ContinuousDelay2< float >( 
                                                               "cont_ref_delay\0",
                                                               CommSystem,
                                                               ref_sig,
                                                               delayed_ref_sig);

//   DiscreteDelay< float >* ref_delay = new DiscreteDelay<float>( "ref_delay\0",
//                                                                 CommSystem,
//                                                                 ref_sig,
//                                                                 delayed_ref_sig);

   DiscreteDelay< float >* filt_delay = new DiscreteDelay<float>( "filt_delay\0",
                                                                 CommSystem,
                                                                 filt_sig,
                                                                 delayed_filt_sig);

   MeanSquareError< float >* error_meter = new MeanSquareError<float>( 
                                                   "error_meter\0",
                                                   CommSystem,
                                                   delayed_filt_sig,
                                                   delayed_ref_sig);

   SignalAnchor* temp_ref_anchor = new SignalAnchor( "temp_ref_anchor\0",
                                                      CommSystem,
                                                      ref_sig);

   SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig);
  //=============================================================
  #include "sim_postamble.cpp"

  return 0;
}  
