//
//  File = qpsk_sim.cpp
//

#define SIM_NAME "QpskSim\0"
#define SIM_TITLE "Complex Baseband Simulation of QPSK\0"

#include "global_stuff.h" 

#include "qpsk_theory.h"
#include "bitgen.h"
#include "bitwav.h"
#include "discrete_delay_T.h"
#include "siganchr.h"
#include "quadmod.h"
#include "phase_rotate.h"
#include "add_gaus_noise.h"
#include "spec_analyzer.h"
#include "phase_genie.h"
#include "quaddem.h"
//#include "bitslicer.h"
#include "integ_dump_slice.h"
#include "qpskoptbitdem.h"
#include "ber_ctr.h"
#include "qpsk_err_ctr.h"

main()
{
#include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

  //-----------------------------------------------------
  // create plot file of theoretical PSD for QPSK signal

  QpskPsd(  0.0,  // carrier_freq
            1.0,  // symb_energy,
            1.0,  // symb_intvl,
            -4.0, // beg_freq,
            4.0,  // end_freq,
            801,  // num_pts,
            true, // plot_in_db,
            "qpsk_psd_theor.txt\0"); //out_filename

  //-----------------------------------------------------
  // create plot file of BER for ideal QPSK

  QpskBer(  -10.0,  // beg_ebno,
            18.0, // end_ebno,
            401,  // num_pts,
            "theor_qpsk_ber.txt\0");  // out_filename
  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(i_bits);
  BIT_SIGNAL(q_bits);
  FLOAT_SIGNAL(i_baseband_wave);
  FLOAT_SIGNAL(q_baseband_wave);
  COMPLEX_SIGNAL(modulated_signal);
  COMPLEX_SIGNAL(shifted_signal);
  COMPLEX_SIGNAL(recov_carrier_sig);
  COMPLEX_SIGNAL(noisy_sig);
  BIT_SIGNAL(symb_clock);
  BIT_SIGNAL(delayed_clock);
  BIT_SIGNAL(q_symb_clock);
  FLOAT_SIGNAL(power_meas_sig);
  FLOAT_SIGNAL(i_demod_wave);
  FLOAT_SIGNAL(q_demod_wave);
  FLOAT_SIGNAL(i_samp_wave);
  FLOAT_SIGNAL(q_samp_wave);
  BIT_SIGNAL(i_bit_decis);
  BIT_SIGNAL(q_bit_decis);

  //============================================================
  //  Construct, initialize and connect models


  BitGener* i_bit_gen = new BitGener( "i_bit_gen\0",
                                      CommSystem,
                                      i_bits );

  BitGener* q_bit_gen = new BitGener( "q_bit_gen\0",
                                      CommSystem,
                                      q_bits );

  BitsToWave* i_wave_gen = new BitsToWave( "i_wave_gen\0",
                                           CommSystem,
                                           i_bits,
                                           i_baseband_wave,
                                           symb_clock );

  BitsToWave* q_wave_gen = new BitsToWave( "q_wave_gen\0",
                                           CommSystem,
                                           q_bits,
                                           q_baseband_wave,
                                           q_symb_clock );

  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                i_baseband_wave,
                                                q_baseband_wave,
                                                0.0625, //samp_intvl
                                                4096 ); //block_size

  QuadratureModulator* qpsk_mod = new QuadratureModulator( "qpsk_mod\0",
                                                CommSystem,
                                                i_baseband_wave,
                                                q_baseband_wave,
                                                modulated_signal);
  
  PhaseRotate* phase_shifter = new PhaseRotate( "phase_shifter\0",
                                                CommSystem,
                                                modulated_signal,
                                                shifted_signal);

  AdditiveGaussianNoise<std::complex<float> >* agn_source = 
                  new AdditiveGaussianNoise<std::complex<float> >( 
                                                "agn_source\0",
                                                CommSystem,
                                                shifted_signal,
                                                noisy_sig,
                                                power_meas_sig);

  SpectrumAnalyzer<std::complex<float> >* spec_analyzer = 
                  new SpectrumAnalyzer<std::complex<float> >( 
                                                  "spec_analyzer\0",
                                                  CommSystem,
                                                  modulated_signal );

  PhaseRecoveryGenie* carrier_recovery = new PhaseRecoveryGenie( "carrier_recovery\0",
                                                                   CommSystem,
                                                                   recov_carrier_sig);

//  QpskOptimalBitDemod* qpsk_dem = new QpskOptimalBitDemod( "qpsk_dem\0",
//                                                    CommSystem,
//                                                    noisy_sig,
//                                                    recov_carrier_sig,
//                                                    symb_clock,
//                                                    i_bit_decis,
//                                                    q_bit_decis);
  
  QuadratureDemod* quad_dem = new QuadratureDemod( "quad_dem\0",
                                                   CommSystem,
                                                   noisy_sig,
                                                   recov_carrier_sig,
                                                   i_demod_wave,
                                                   q_demod_wave );

  IntegrateDumpAndSlice* i_bit_slicer = new IntegrateDumpAndSlice( "i_bit_slicer\0",
                                           CommSystem,
                                           i_demod_wave,
                                           symb_clock,
                                           i_samp_wave,
                                           i_bit_decis);

  IntegrateDumpAndSlice* q_bit_slicer = new IntegrateDumpAndSlice( "q_bit_slicer\0",
                                           CommSystem,
                                           q_demod_wave,
                                           q_symb_clock,
                                           q_samp_wave,
                                           q_bit_decis);

  BerCounter* i_ber_ctr = new BerCounter( "i_ber_ctr\0",
                                          CommSystem,
                                          i_bits,
                                          i_bit_decis );

  BerCounter* q_ber_ctr = new BerCounter( "q_ber_ctr\0",
                                          CommSystem,
                                          q_bits,
                                          q_bit_decis );

  QpskErrorCounter* ser_ctr = new QpskErrorCounter( "ser_ctr\0",
                                                    CommSystem,
                                                    i_bits,
                                                    q_bits,
                                                    i_bit_decis,
                                                    q_bit_decis );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
