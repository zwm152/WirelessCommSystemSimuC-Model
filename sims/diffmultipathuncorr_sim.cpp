//
//  File = diffmultipathuncorr_sim.cpp
//
#define SIM_NAME "DiffMultipathUncorr\0"
#define SIM_TITLE "Diffuse Multipath Channel Uncorrelated Tap Gains\0"

#include "global_stuff.h"

//=====================
// model includes
#include "bitgen.h"
#include "bitwav.h"
#include "siganchr.h"
#include "quadmod.h"
#include "phase_genie.h"
#include "quaddem.h"
#include "integ_dump_slice.h"
#include "qpskoptbitdem.h"
#include "ber_ctr.h"
#include "qpsk_err_ctr.h"
#include "chan_diff_mp_uncorr.h"
#include "cpxtophs.h"
//=========================================================

main()
{
   #include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

   //CorrTapTest( "R_matrix.txt\0");


  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(i_bits);
  BIT_SIGNAL(q_bits);
  FLOAT_SIGNAL(i_baseband_wave);
  FLOAT_SIGNAL(q_baseband_wave);
  COMPLEX_SIGNAL(modulated_signal);
  COMPLEX_SIGNAL(distorted_signal);
  COMPLEX_SIGNAL(recov_carrier_sig);
  BIT_SIGNAL(symb_clock);
  BIT_SIGNAL(delayed_clock);
  BIT_SIGNAL(q_symb_clock);
  FLOAT_SIGNAL(i_demod_wave);
  FLOAT_SIGNAL(q_demod_wave);
  FLOAT_SIGNAL(i_samp_wave);
  FLOAT_SIGNAL(q_samp_wave);
  BIT_SIGNAL(i_bit_decis);
  BIT_SIGNAL(q_bit_decis);
  FLOAT_SIGNAL(phase_sig_1);
  FLOAT_SIGNAL(mag_sig_1);
  FLOAT_SIGNAL(phase_sig_2);
  FLOAT_SIGNAL(mag_sig_2);


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

   CmpxToPhase* phase_det_1 = new CmpxToPhase( "phase_det_1\0",
                                                CommSystem,
                                                modulated_signal,
                                                phase_sig_1,
                                                mag_sig_1);

   ChanDiffuseMultipathUncorr* channel = new ChanDiffuseMultipathUncorr( 
                                                "channel\0",
                                                CommSystem,
                                                modulated_signal,
                                                distorted_signal);

   CmpxToPhase* phase_det_2 = new CmpxToPhase( "phase_det_2\0",
                                                CommSystem,
                                                distorted_signal,
                                                phase_sig_2,
                                                mag_sig_2);

  PhaseRecoveryGenie* carrier_recovery = new PhaseRecoveryGenie( "carrier_recovery\0",
                                                                   CommSystem,
                                                                   recov_carrier_sig);


  QuadratureDemod* quad_dem = new QuadratureDemod( "quad_dem\0",
                                                   CommSystem,
                                                   distorted_signal,
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
