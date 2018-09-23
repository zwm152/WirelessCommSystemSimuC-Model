//
//  File = fsk_sim_bp.cpp
//

#define SIM_NAME "FskSim_Bp\0"
#define SIM_TITLE "Bandpass Simulation of FSK\0"

#include "global_stuff.h" 

#include "fsk_theory.h"
#include "gausrand.h"
#include "bitgen.h"
#include "bitwav.h"
#include "discrete_delay_T.h"
#include "fsk_2tone_bp.h"
#include "add_gaus_noise.h"
#include "spec_analyzer.h"
#include "fsk_genie.h"
#include "fsk_demod_bp.h"
//#include "correlator.h"
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

   //-----------------------------------------------------
   // create plot of correlation coefficient

   FskCorrCoeff( 1.0,  //  bit_intvl,
                 -1.5, // beg_freq,
                  1.5, // end_freq,
                  201, // num_pts,
                  "fsk_corr_coeff.txt\0"); // out_filename

  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_vals);
  BIT_SIGNAL(delayed_bits);
  FLOAT_SIGNAL(baseband_wave);
  FLOAT_SIGNAL(filt_baseband_wave);
  FLOAT_SIGNAL(modulated_signal);
  FLOAT_SIGNAL(noisy_sig);
  FLOAT_SIGNAL(lo_integ_sig);
  FLOAT_SIGNAL(lo_rx_cross_corr);
  FLOAT_SIGNAL(hi_integ_sig);
  FLOAT_SIGNAL(lo_fsk_filt_sig);
  FLOAT_SIGNAL(hi_fsk_filt_sig);  
  BIT_SIGNAL(symb_clock);
  BIT_SIGNAL(delayed_clock);
  FLOAT_SIGNAL(power_meas_sig);
  BIT_SIGNAL(bit_decis);

  //============================================================
  //  Construct, initialize and connect models


  BitGener* bit_gen = new BitGener( "bit_gen\0",
                                      CommSystem,
                                      bit_vals );

  DiscreteDelay< bit_t >* bit_delay = new DiscreteDelay< bit_t >( "bit_delay\0",
                                                                CommSystem,
                                                                bit_vals,
                                                                delayed_bits );

  BitsToWave* wave_gen = new BitsToWave( "wave_gen\0",
                                           CommSystem,
                                           bit_vals,
                                           baseband_wave,
                                           symb_clock );

//  ButterworthFilterByIir<float>* tx_baseband_filter = 
//            new ButterworthFilterByIir<float>( "tx_baseband_filter\0",
//                                        CommSystem,
//                                        baseband_wave,
//                                        filt_baseband_wave );

  DiscreteDelay< bit_t >* clock_delay = new DiscreteDelay< bit_t >( "clock_delay\0",
                                                                CommSystem,
                                                                symb_clock,
                                                                delayed_clock );

  FskTwoToneModulator_Bp* fsk_mod = new FskTwoToneModulator_Bp( "fsk_mod\0",
                                                CommSystem,
                                                baseband_wave,
                                                modulated_signal);
  
  AdditiveGaussianNoise<float>* agn_source = 
                  new AdditiveGaussianNoise<float>( 
                                                "agn_source\0",
                                                CommSystem,
                                                modulated_signal,
                                                noisy_sig,
                                                power_meas_sig);

  SpectrumAnalyzer<float>* modulator_spec_an = 
                  new SpectrumAnalyzer<float>( 
                                              "modulator_spec_an\0",
                                              CommSystem,
                                              modulated_signal );

//  FskCarrierGenie* carrier_recovery = new FskCarrierGenie( "carrier_recovery\0",
//                                                           CommSystem,
//                                                           lo_ref_sig,
//                                                           hi_ref_sig);

  FskBandpassDemod* fsk_dem = new FskBandpassDemod( "fsk_dem\0",
                                                    CommSystem,
                                                    noisy_sig,
                                                    delayed_clock,
                                                    bit_decis,
                                                    lo_fsk_filt_sig,
                                                    hi_fsk_filt_sig,
                                                    lo_integ_sig,
                                                    hi_integ_sig);
  
//  Control<float>* lo_ref_delay_ctl = new Control<float>("lo_ref_delay_ctl\0");
//  Control<int>* lo_ref_samps_delay_ctl = new Control<int>("lo_ref_samps_delay_ctl\0");

//   RealCorrelator* lo_timing_corr = new RealCorrelator( "lo_timing_corr\0",
//                                                     CommSystem,
//                                                     lo_integ_sig,
//                                                     filt_baseband_wave,
//                                                     lo_rx_cross_corr,
//                                                     lo_ref_delay_ctl,
//                                                     lo_ref_samps_delay_ctl);

  SpectrumAnalyzer<float>* lo_filt_spec_an = 
                  new SpectrumAnalyzer<float>( 
                                              "lo_filt_spec_an\0",
                                              CommSystem,
                                              lo_fsk_filt_sig );

  SpectrumAnalyzer<float>* hi_filt_spec_an = 
                  new SpectrumAnalyzer<float>( 
                                              "hi_filt_spec_an\0",
                                              CommSystem,
                                              hi_fsk_filt_sig );

  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                baseband_wave);

  BerCounter* ber_ctr = new BerCounter( "ber_ctr\0",
                                          CommSystem,
                                          delayed_bits,
                                          bit_decis );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
