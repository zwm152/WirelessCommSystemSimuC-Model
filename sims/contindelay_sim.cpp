//
//  File = contindelay_sim.cpp
//
#define SIM_NAME "ContinDelay\0"
#define SIM_TITLE "Continuous Delay Testbed\0"

#include "global_stuff.h"

//=====================
// model includes
#include "sinc_wave.h"
#include "bitgen.h"
#include "bitwav.h"
#include "multi_tone_gen.h"
#include "ramp_gen.h"
#include "contin_delay_tester.h"
#include "contin_delay_T.h"
#include "dft_delay_T.h"
#include "siganchr.h"
//=========================================================

main()
{
   #include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

   SincWaveform(  4, //num_sidelobes
                 50, //samps_per_hump
                 "sinc_wave.txt\0");//out_filename


  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);
  FLOAT_SIGNAL(i_test_sig);
  FLOAT_SIGNAL(q_test_sig);
  FLOAT_SIGNAL(test_sig);
  //FLOAT_SIGNAL(calib_sig);
  FLOAT_SIGNAL(test_sig_2);
  FLOAT_SIGNAL(delayed_test_sig);

  FLOAT_SIGNAL(baseband_wave);

  //============================================================
  //  Construct, initialize and connect models


  MultipleToneGener* sine_gen_1 = new MultipleToneGener( "sine_gen_1\0",
                                          CommSystem,
                                          test_sig );

  //MultipleSineGener* sine_gen_3 = new MultipleSineGener( "sine_gen_3\0",
  //                                        CommSystem,
  //                                        calib_sig );


   //RampGener* ramp_gen = new RampGener(   "ramp_gen\0",
   //                                       CommSystem,
   //                                       test_sig );

  Control<bool>* delay_chg_enab = new Control<bool>( "delay_chg_enab\0",
                                                   CommSystem );

  Control<float>* dynam_float_dly = new Control<float>( "dynam_float_dly\0",
                                                   CommSystem );
  ContinuousDelayTester< float >* dyn_delay_knob = 
                            new ContinuousDelayTester< float >( 
                                                  "dyn_delay_knob\0",
                                                  CommSystem,
                                                  test_sig,
                                                  test_sig_2,
                                                  dynam_float_dly,
                                                  delay_chg_enab );
  
  ContinuousDelay< float >* delay_2 = new ContinuousDelay< float >( 
                                                "delay_2\0",
                                                CommSystem,
                                                test_sig_2,
                                                delayed_test_sig,
                                                dynam_float_dly,
                                                delay_chg_enab);


//  RateChanger* rate_changer = new RateChanger("rate_changer\0",
//                                                       CommSystem,
//                                                       test_sig,
//                                                       baseband_wave);


  SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig,
                                                0.0078125, //samp_intvl
                                                4096 ); //block_size
//  SignalAnchor* temp_anchor_3 = new SignalAnchor( "temp_anchor_3\0",
//                                                CommSystem,
//                                                ref_seq,
//                                                1.0, //samp_rate
//                                                128 ); //block_size
  //=============================================================

  #include "sim_postamble.cpp"
  return 0;
}  
