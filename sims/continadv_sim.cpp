//
//  File = delay_testbed_sim.cpp
//
#define SIM_NAME "DelayTestbed\0"
#define SIM_TITLE "Delay Testbed\0"

#include "global_stuff.h"

//=====================
// model includes
#include "bitgen.h"
#include "bitwav.h"
#include "multi_tone_gen.h"
#include "quadcmpx.h"
#include "disc_delay_tester.h"
#include "contin_delay_tester.h"
#include "discrete_delay_T.h"
#include "contin_delay_T.h"
#include "contin_adv_T.h"
#include "discrete_adv_T.h"
#include "rate_changer.h"
#include "siganchr.h"
//=========================================================

main()
{
   #include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing


  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);
  FLOAT_SIGNAL(i_test_sig);
  FLOAT_SIGNAL(q_test_sig);
  FLOAT_SIGNAL(test_sig);
  FLOAT_SIGNAL(test_sig_2);
  FLOAT_SIGNAL(advanced_test_sig);

  FLOAT_SIGNAL(baseband_wave);

  //============================================================
  //  Construct, initialize and connect models

 /// EnclaveNumber = 0;

//  StairGener* stair_gen = new StairGener( "stair_gen\0",
//                                      CommSystem,
//                                      test_sig );

  MultipleToneGener* sine_gen_1 = new MultipleToneGener( "sine_gen_1\0",
                                          CommSystem,
                                          test_sig );

//  SineGener* sine_gen_2 = new SineGener( "sine_gen_2\0",
//                                          CommSystem,
//                                          q_test_sig );

//  QuadratureToComplex* iq_combiner = new QuadratureToComplex( "iq_combiner\0",
//                                                            CommSystem,
//                                                            i_test_sig,
//                                                            q_test_sig,
//                                                            test_sig);
//  BitGener* bit_gen = new BitGener( "bit_gen\0",
//                                      CommSystem,
//                                      bit_seq );

//  BitsToWave* wave_gen = new BitsToWave( "wave_gen\0",
//                                           CommSystem,
//                                           bit_seq,
//                                           test_sig );

  Control<bool>* delay_chg_enab = new Control<bool>( "delay_chg_enab\0",
                                                   CommSystem );
//  Control<int>* dynam_int_adv = new Control<int>( "dynam_int_adv\0",
//                                                   CommSystem );
  Control<float>* dynam_float_adv = new Control<float>( "dynam_float_adv\0",
                                                   CommSystem );

//  DiscreteDelayTester< float >* dyn_delay_knob = new DiscreteDelayTester<float>( 
//                                                  "dyn_delay_knob\0",
//                                                  CommSystem,
//                                                  test_sig,
//                                                  test_sig_2,
//                                                  dynam_int_adv,
//                                                  delay_chg_enab );

//  DiscreteAdvance<float>* advance_1 = new DiscreteAdvance<float>( 
//                                                  "advance_1\0",
//                                                  CommSystem,
//                                                  test_sig_2,
//                                                  advanced_test_sig);
//                                                  advanced_test_sig,
//                                                  dynam_int_adv,
//                                                  delay_chg_enab );
//  Control<bool>* delay_chg_enab = new Control<bool>( "delay_chg_enab\0",
//                                                   CommSystem );
//  Control<int>* dynam_int_dly = new Control<int>( "dynam_int_dly\0",
//                                                   CommSystem );
//  DiscreteDelayTester< float >* dyn_delay_knob = new DiscreteDelayTester<float>( 
//                                                  "dyn_delay_knob\0",
//                                                  CommSystem,
//                                                  test_sig,
//                                                  test_sig_2,
//                                                  dynam_int_dly,
//                                                  delay_chg_enab );
  
//  DiscreteDelay< float >* delay_1 = new DiscreteDelay<float>( 
//                                                "delay_1\0",
//                                                CommSystem,
//                                                test_sig_2,
//                                                delayed_test_sig,
//                                                dynam_int_dly,
///                                                delay_chg_enab);

//  Control<float>* dynam_float_dly = new Control<float>( "dynam_float_dly\0",
//                                                   CommSystem );
  ContinuousDelayTester< float >* dyn_delay_knob = 
                            new ContinuousDelayTester< float >( 
                                                  "dyn_delay_knob\0",
                                                  CommSystem,
                                                  test_sig,
                                                  test_sig_2,
                                                  dynam_float_adv,
                                                  delay_chg_enab );
  
  ContinuousAdvance<float>* advance_2 = new ContinuousAdvance<float>( 
                                                  "advance_2\0",
                                                  CommSystem,
                                                  test_sig_2,
                                                  advanced_test_sig);
//  ContinuousDelay< std::complex<float> >* delay_2 = 
//                              new ContinuousDelay< std::complex<float> >( 
//                                                "delay_2\0",
//                                                CommSystem,
//                                                test_sig_2,
//                                                delayed_test_sig,
//                                                dynam_float_dly,
//                                                delay_chg_enab);


//  RateChanger* rate_changer = new RateChanger("rate_changer\0",
//                                                       CommSystem,
//                                                       test_sig,
//                                                       baseband_wave);


  SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig,
                                                0.0078125, //samp_rate
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
