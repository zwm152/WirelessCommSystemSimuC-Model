//
//  File = fsk_coh_sim.cpp
//

#define SIM_NAME "FskCohSim\0"
#define SIM_TITLE "Bandpass Sim of Coherent FSK\0"

#include "global_stuff.h" 

#include "fsk_theory.h"
#include "gausrand.h"
#include "bitgen.h"
#include "bitwav.h"
#include "butt_filt_iir.h"
#include "vco_ce.h"
#include "add_gaus_noise.h"
#include "spec_analyzer.h"
#include "fsk_genie.h"
#include "fsk_demod_coh.h"
#include "ber_ctr.h"
#include "siganchr.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //-----------------------------------------------------
  // create plot file of theoretical PSD for FSK signal

  FskPsd(  0.0,  // carrier_freq
            1.0,  // symb_energy,
            1.0,  // symb_intvl,
            -4.0, // beg_freq,
            4.0,  // end_freq,
            801,  // num_pts,
            true, // plot_in_db,
            "fsk_psd_theor.txt\0"); //out_filename

  //-----------------------------------------------------
  // create plot file of BER for ideal QPSK

  FskBer(  -10.0,  // beg_ebno,
            18.0, // end_ebno,
            401,  // num_pts,
            "theor_fsk_ber.txt\0");  // out_filename
  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_vals);
  FLOAT_SIGNAL(baseband_wave);
  FLOAT_SIGNAL(filt_baseband_wave);
  COMPLEX_SIGNAL(modulated_signal);
  FLOAT_SIGNAL(modulated_ref_signal);
  COMPLEX_SIGNAL(noisy_sig);
  COMPLEX_SIGNAL(lo_ref_sig);
  COMPLEX_SIGNAL(hi_ref_sig);
  BIT_SIGNAL(symb_clock);
  BIT_SIGNAL(delayed_clock);
  FLOAT_SIGNAL(power_meas_sig);
  BIT_SIGNAL(bit_decis);
  FLOAT_SIGNAL(lo_integ_sig);
  FLOAT_SIGNAL(hi_integ_sig);
  FLOAT_SIGNAL(lo_correl_sig);
  FLOAT_SIGNAL(hi_correl_sig);

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


   ComplexVco* fsk_mod_1 = new ComplexVco( "fsk_mod_1\0",
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

  SpectrumAnalyzer<std::complex<float> >* modulator_spec_an = 
                  new SpectrumAnalyzer<std::complex<float> >( 
                                              "modulator_spec_an\0",
                                              CommSystem,
                                              modulated_signal );

  FskCarrierGenie* carrier_recovery = new FskCarrierGenie( "carrier_recovery\0",
                                                           CommSystem,
                                                           lo_ref_sig,
                                                           hi_ref_sig);


  FskCoherentDemod* fsk_dem = new FskCoherentDemod( "fsk_dem\0",
                                                    CommSystem,
                                                    noisy_sig,
                                                    lo_ref_sig,
                                                    hi_ref_sig,
                                                    symb_clock,
                                                    bit_decis,
                                                    lo_integ_sig,
                                                    hi_integ_sig);
  
  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                baseband_wave);

  BerCounter* ber_ctr = new BerCounter( "ber_ctr\0",
                                          CommSystem,
                                          bit_vals,
                                          bit_decis );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
