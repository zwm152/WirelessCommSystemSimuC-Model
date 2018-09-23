//
//  File = symbseqtest_sim.cpp
//

#define SIM_NAME "SymbSeqTest\0"
#define SIM_TITLE "Symbol Sequence Testbed\0"

#include "global_stuff.h" 

#include "symbgen.h"
#include "symb_analyzer.h"
#include "siganchr.h"

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
  
   SymbGener* symb_gen = new SymbGener( "symb_gen\0",
                                      CommSystem,
                                      symb_seq );
  
   SignalAnchor* symb_seq_anchr = new SignalAnchor( "symb_seq_anchr\0",
                                                   CommSystem,
                                                   symb_seq );

   SymbSeqAnalyzer* symb_anlyzr = new SymbSeqAnalyzer(   "symb_anlyzr\0",
                                                      CommSystem,
                                                      symb_seq );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
