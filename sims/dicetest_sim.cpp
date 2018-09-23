//
//  File = dice_test_sim.cpp
//

#define SIM_NAME "DiceTest\0"
#define SIM_TITLE "Dice Rolling Testbed\0"

#include "global_stuff.h" 

#include "dice.h"
#include "dice_analyzer.h"
#include "siganchr.h"
#include "disc_auto_cov.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing


  //=========================================================================
  //  Allocate signals

  BYTE_SIGNAL(symb_seq);

  //============================================================
  //  Construct, initialize and connect models


   DiceRoller* dice_roller = new DiceRoller( "dice_roller\0",
                                      CommSystem,
                                      symb_seq );
  
   SignalAnchor* bit_seq_anchr = new SignalAnchor( "bit_seq_anchr\0",
                                                   CommSystem,
                                                   symb_seq );

   DiceAnalyzer* dice_anlyzr = new DiceAnalyzer(   "dice_anlyzr\0",
                                                      CommSystem,
                                                      symb_seq );

   DiscreteAutoCovar* disc_corr = new DiscreteAutoCovar( "disc_covar\0",
                                                        CommSystem,
                                                        symb_seq);

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
