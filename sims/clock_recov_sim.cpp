//
//  File = clock_recov_sim.cpp
//

#define SIM_NAME "ClockRecov\0"
#define SIM_TITLE "Simulation of Clock Recovery Loop\0"

#include "global_stuff.h" 

#include "bitgen.h"
#include "bitwav.h"
#include "siganchr.h"
#include "bpsk_mod_bp.h"
#include "phase_rotate.h"
#include "add_gaus_noise.h"
#include "butt_filt_iir.h"
#include "spec_analyzer.h"
#include "quad_carr_genie.h"
#include "clock_recov.h"
#include "costas_loop.h"
#include "contin_delay_T.h"
#include "mixer_bp.h"
#include "quad_mixer_bp.h"
#include "integ_dump_slice.h"
#include "qpskoptbitdem.h"
#include "ber_ctr.h"
#include "qpsk_err_ctr.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_vals);
  FLOAT_SIGNAL(baseband_wave);
  FLOAT_SIGNAL(modulated_signal);
  FLOAT_SIGNAL(pll_err_sig);
  FLOAT_SIGNAL(pll_filt_err_sig);
  FLOAT_SIGNAL(cr_loop_raw_err_sig);
  FLOAT_SIGNAL(cr_loop_filt_err_sig);
  FLOAT_SIGNAL(pll_ref_sig);
  FLOAT_SIGNAL(pll_vco_phase_sig);
  FLOAT_SIGNAL(pll_vco_freq_sig);
  FLOAT_SIGNAL(squared_signal);
  FLOAT_SIGNAL(error_signal);
  FLOAT_SIGNAL(filt_squared_signal);
  FLOAT_SIGNAL(recov_clock_sig);
  FLOAT_SIGNAL(recov_carrier_sig);
  FLOAT_SIGNAL(cr_recov_symb_period);
  FLOAT_SIGNAL(demod_sig);
  FLOAT_SIGNAL(noisy_sig);
  //FLOAT_SIGNAL(sqrd_noisy_sig);
  BIT_SIGNAL(symb_clock);
  FLOAT_SIGNAL(power_meas_sig);

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

  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                baseband_wave,
                                                //q_baseband_wave,
                                                0.0078125, //samp_intvl
                                                4096 ); //block_size

  BpskBandpassModulator* bpsk_mod = new BpskBandpassModulator( "bpsk_mod\0",
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

  SpectrumAnalyzer<float>* spec_analyzer = 
                  new SpectrumAnalyzer<float>( 
                                                  "spec_analyzer\0",
                                                  CommSystem,
                                                  modulated_signal );

//   BandpassSquaringPLL* carr_recov_loop = new BandpassSquaringPLL( "carr_recov_loop\0",
//                                                                     CommSystem,
//                                                                     noisy_sig,
//                                                                     sqrd_noisy_sig,
//                                                                     pll_err_sig,
//                                                                     pll_filt_err_sig,
//                                                                     pll_ref_sig,
//                                                                     pll_vco_freq_sig,
//                                                                     pll_phase_sig,
//                                                                     recov_carrier_sig);

//   ContinuousDelay< float >* loop_delay_elem = new ContinuousDelay<float>( "loop_delay_elem\0",
//                                   CommSystem,
//                                   squared_signal,
//                                   delay_squared_signal);


   CostasLoop* carr_recov_loop = new CostasLoop( "carr_recov_loop\0",
                                                CommSystem,
                                                modulated_signal,
                                                pll_err_sig,
                                                demod_sig,
                                                pll_ref_sig,
                                                pll_vco_freq_sig,
                                                pll_vco_phase_sig,
                                                recov_carrier_sig);

   ClockRecoveryLoop* clock_recov_loop = new ClockRecoveryLoop( "clock_recov_loop\0",
                                                               CommSystem,
                                                               baseband_wave,
                                                               cr_loop_raw_err_sig,
                                                               cr_loop_filt_err_sig,
                                                               cr_recov_symb_period,
                                                               recov_clock_sig);

//  ButterworthFilterByIir<float>* double_freq_filter = 
//            new ButterworthFilterByIir<float>( "double_freq_filter\0",
//                                        CommSystem,
//                                        squared_signal,
//                                        filt_squared_signal );

//  SpectrumAnalyzer<float>* spec_anal_2 = 
//                  new SpectrumAnalyzer<float>( 
//                                                  "spec_anal_2\0",
//                                                  CommSystem,
//                                                  sqrd_noisy_sig );

//  SpectrumAnalyzer<float>* spec_anal_3 = 
//                  new SpectrumAnalyzer<float>( 
//                                                  "spec_anal_3\0",
//                                                  CommSystem,
//                                                  pll_err_sig );

  SpectrumAnalyzer<float>* spec_anal_4 = 
                  new SpectrumAnalyzer<float>( 
                                                  "spec_anal_4\0",
                                                  CommSystem,
                                                  demod_sig );

//  QuadCarrierRecovGenie* carrier_recovery = new QuadCarrierRecovGenie( "carrier_recovery\0",
//                                                                   CommSystem,
//                                                                   i_recov_carrier_sig,
//                                                                   q_recov_carrier_sig);
  
//  QuadBandpassMixer* quad_dem = new QuadBandpassMixer( "quad_dem\0",
//                                                   CommSystem,
//                                                   noisy_sig,
//                                                   i_recov_carrier_sig,
//                                                   q_recov_carrier_sig,
//                                                   i_demod_wave,
//                                                   q_demod_wave );

//  IntegrateDumpAndSlice* i_bit_slicer = new IntegrateDumpAndSlice( "i_bit_slicer\0",
//                                           CommSystem,
//                                           i_demod_wave,
//                                           symb_clock,
//                                           i_samp_wave,
//                                           i_bit_decis);

//  IntegrateDumpAndSlice* q_bit_slicer = new IntegrateDumpAndSlice( "q_bit_slicer\0",
//                                           CommSystem,
//                                           q_demod_wave,
//                                           q_symb_clock,
//                                           q_samp_wave,
//                                           q_bit_decis);

//  BerCounter* i_ber_ctr = new BerCounter( "i_ber_ctr\0",
//                                          CommSystem,
//                                          i_bits,
//                                          i_bit_decis );

//  BerCounter* q_ber_ctr = new BerCounter( "q_ber_ctr\0",
//                                          CommSystem,
//                                          q_bits,
//                                          q_bit_decis );

//  QpskErrorCounter* ser_ctr = new QpskErrorCounter( "ser_ctr\0",
//                                                    CommSystem,
//                                                    i_bits,
//                                                    q_bits,
//                                                    i_bit_decis,
//                                                    q_bit_decis );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
