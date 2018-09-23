//
//  File = bitseqtest_sim.cpp
//

#define SIM_NAME "BitSeqTest\0"
#define SIM_TITLE "Bit Sequence Testbed\0"

#include "global_stuff.h" 

#include "bitgen.h"
#include "bit_analyzer.h"
#include "siganchr.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);

  //============================================================
  //  Construct, initialize and connect models


   BitGener* bit_gen = new BitGener( "bit_gen\0",
                                      CommSystem,
                                      bit_seq );
    
   SignalAnchor* bit_seq_anchr = new SignalAnchor( "bit_seq_anchr\0",
                                                   CommSystem,
                                                   bit_seq );

   BitSeqAnalyzer* bit_anlyzr = new BitSeqAnalyzer(   "bit_anlyzr\0",
                                                      CommSystem,
                                                      bit_seq );


  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
