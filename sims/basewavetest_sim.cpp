//
//  File = basewavetest_sim.cpp
//

#define SIM_NAME "BaseWaveTest\0"
#define SIM_TITLE "Baseband Waveform Testbed\0"

#include "global_stuff.h" 

#include "rect_pulse_theory.h"
#include "bitgen.h"
#include "nrz_theory.h"
#include "basewav.h"
#include "siganchr.h"
#include "spec_analyzer.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  RectPulseAliasPower( 64.0,
                       100,
                       "rect_pulse_pwr.txt\0");

   NrzPsd(  1.0, // bit energy
            1.0, // bit_rate,
            0.0, // beg_freq,
            4.0, // end_freq,
            401, // num_pts,
            true, //plot_in_db,
            "nrz_psd_theor.txt\0");

   BiphasePsd(  1.0, // bit energy
                  1.0, // bit_rate,
                  0.0, // beg_freq,
                  4.0, // end_freq,
                  401, // num_pts,
                  true, //plot_in_db,
                  "biphase_psd_theor.txt\0");

   MillerPsd(  1.0, // bit energy
                  1.0, // bit_rate,
                  0.0, // beg_freq,
                  4.0, // end_freq,
                  401, // num_pts,
                  true, //plot_in_db,
                  "miller_psd_theor.txt\0");
  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);
  FLOAT_SIGNAL(binary_wave);
  BIT_SIGNAL(bit_clock);

  //============================================================
  //  Construct, initialize and connect models


   BitGener* bit_gen = new BitGener( "bit_gen\0",
                                      CommSystem,
                                      bit_seq );
    
   SignalAnchor* bit_seq_anchr = new SignalAnchor( "bit_seq_anchr\0",
                                                   CommSystem,
                                                   bit_seq );

   BasebandWaveform* baseband_gen = new BasebandWaveform(   "baseband_gen\0",
                                                CommSystem,
                                                bit_seq,
                                                binary_wave,
                                                bit_clock );

   SpectrumAnalyzer<float>* spec_analyzer = 
                  new SpectrumAnalyzer<float>(  "spec_analyzer\0",
                                                CommSystem,
                                                binary_wave );


  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
