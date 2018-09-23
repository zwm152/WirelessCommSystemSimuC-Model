//
//  File = discadv_sim.cpp
//
#define SIM_NAME "DiscAdv\0"
#define SIM_TITLE "Discrete Advance Testbed 2\0"

#include "global_stuff.h"

//=====================
// model includes
#include "bitgen.h"
#include "bitwav.h"
#include "disc_delay_tester.h"
#include "discrete_adv_T.h"
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
  BIT_SIGNAL(bit_clock);
  FLOAT_SIGNAL(test_sig);
  FLOAT_SIGNAL(test_sig_2);
  FLOAT_SIGNAL(advanced_test_sig);

  FLOAT_SIGNAL(baseband_wave);

  //============================================================
  //  Construct, initialize and connect models

 /// EnclaveNumber = 0;

  BitGener* bit_gen = new BitGener( "bit_gen\0",
                                      CommSystem,
                                      bit_seq );

  BitsToWave* wave_gen = new BitsToWave( "wave_gen\0",
                                           CommSystem,
                                           bit_seq,
                                           test_sig,
                                           bit_clock );

//  Control<bool>* delay_chg_enab = new Control<bool>( "delay_chg_enab\0",
//                                                   CommSystem );
//  Control<int>* dynam_int_adv = new Control<int>( "dynam_int_adv\0",
//                                                   CommSystem );
//  Control<float>* dynam_float_adv = new Control<float>( "dynam_float_adv\0",
//                                                   CommSystem );

//  DiscreteDelayTester< float >* dyn_delay_knob = new DiscreteDelayTester<float>( 
//                                                  "dyn_delay_knob\0",
//                                                  CommSystem,
//                                                  test_sig,
//                                                  test_sig_2,
//                                                  dynam_int_adv,
//                                                  delay_chg_enab );

  DiscreteAdvance<float>* advance_1 = new DiscreteAdvance<float>( 
                                                  "advance_1\0",
                                                  CommSystem,
                                                  test_sig,
                                                  advanced_test_sig);
                                                 // advanced_test_sig,
                                                 // dynam_int_adv,
                                                 // delay_chg_enab );


  SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                test_sig,
                                                0.125, //samp_rate
                                                4096 ); //block_size
  //=============================================================

  #include "sim_postamble.cpp"
  return 0;
}  
