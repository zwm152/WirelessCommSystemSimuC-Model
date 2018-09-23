//
//  File = bpsk_sim.cpp
//

#define SIM_NAME "BpskSim\0"
#define SIM_TITLE "Complex Baseband Simulation of BPSK\0"

#include "global_stuff.h" 

#include "bpsk_theory.h"
#include "gausrand.h"
#include "bitgen.h"
#include "bitwav.h"
#include "bpsk_mod.h"
#include "add_gaus_noise.h"
#include "spec_analyzer.h"
#include "phase_genie.h"
#include "bpsk_correl_demod.h"
#include "ber_ctr.h"
#include "siganchr.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //-----------------------------------------------------
  // create plot file of theoretical PSD for QPSK signal

  BpskPsd(  0.0,  // carrier_freq
            1.0,  // symb_energy,
            1.0,  // symb_intvl,
            -4.0, // beg_freq,
            4.0,  // end_freq,
            801,  // num_pts,
            true, // plot_in_db,
            "bpsk_psd_theor.txt\0"); //out_filename

  //-----------------------------------------------------
  // create plot file of BER for ideal QPSK

  BpskBer(  -10.0,  // beg_ebno,
            18.0, // end_ebno,
            401,  // num_pts,
            "theor_bpsk_ber.txt\0");  // out_filename
  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_vals);
  FLOAT_SIGNAL(baseband_wave);
  BIT_SIGNAL(symb_clock);
  COMPLEX_SIGNAL(modulated_signal);
  COMPLEX_SIGNAL(noisy_sig);
  BIT_SIGNAL(delayed_clock);
  COMPLEX_SIGNAL(phase_ref_sig);
  FLOAT_SIGNAL(power_meas_sig);
  BIT_SIGNAL(bit_decis);

  //============================================================
  //  Construct, initialize and connect models


  BitGener* bit_gen = new BitGener( "bit_gen\0",
                                      CommSystem,
                                      bit_vals );

  BitsToWave* wave_gen = new BitsToWave( "wave_gen\0",
                                           CommSystem,
                                           bit_vals,
                                           baseband_wave,
                                           symb_clock );

  BpskModulator* bpsk_mod = new BpskModulator( "bpsk_mod\0",
                                                CommSystem,
                                                baseband_wave,
                                                modulated_signal);
  
  AdditiveGaussianNoise<std::complex<float> >* agn_source = 
                  new AdditiveGaussianNoise<std::complex<float> >( 
                                                "agn_source\0",
                                                CommSystem,
                                                modulated_signal,
                                                noisy_sig,
                                                power_meas_sig);

  //SpectrumAnalyzer<std::complex<float> >* modulator_spec_an = 
  //                new SpectrumAnalyzer<std::complex<float> >( 
  //                                                "modulator_spec_an\0",
  //                                                CommSystem,
  //                                                modulated_signal );

  PhaseRecoveryGenie* carrier_recovery = new PhaseRecoveryGenie( "carrier_recovery\0",
                                                                   CommSystem,
                                                                   phase_ref_sig);

  BpskCorrelationDemod* bpsk_dem = new BpskCorrelationDemod( "bpsk_dem\0",
                                                    CommSystem,
                                                    noisy_sig,
                                                    phase_ref_sig,
                                                    symb_clock,
                                                    bit_decis);
  
  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                baseband_wave,
                                                0.0625, //samp_intvl
                                                4096 ); //block_size

  BerCounter* ber_ctr = new BerCounter( "ber_ctr\0",
                                          CommSystem,
                                          bit_vals,
                                          bit_decis );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
