//
//  File = binsymchan_sim.cpp
//
#define SIM_NAME "BinSymChan\0"
#define SIM_TITLE "Binary Symmetric Channel Testbed\0"

#include "global_stuff.h"

//=====================
// model includes
#include "bitgen.h"
#include "siganchr.h"
#include "bscerrorgen.h"
#include "ber_ctr.h"
//=========================================================

main()
{
   #include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing



  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);
  BIT_SIGNAL(corrupt_seq);

  //============================================================
  //  Construct, initialize and connect models



   BitGener* bit_gen = new BitGener( "bit_gen\0",
                                      CommSystem,
                                      bit_seq );
  
   SignalAnchor* bit_seq_anchr = new SignalAnchor( "bit_seq_anchr\0",
                                                   CommSystem,
                                                   bit_seq );

   BscErrorGener* err_gen = new BscErrorGener( "err_gen\0",
                                              CommSystem,
                                              bit_seq,
                                              corrupt_seq );
  
  BerCounter* ber_ctr = new BerCounter( "ber_ctr\0",
                                        CommSystem,
                                        bit_seq,
                                        corrupt_seq );
  //=============================================================

  #include "sim_postamble.cpp"
  return 0;
}  
