//
//  File = qam_sim_bp.cpp
//

#define SIM_NAME "QamSim_Bp\0"
#define SIM_TITLE "Bandpass Simulation for QAM\0"

#include "global_stuff.h" 

#include "gausrand.h"
#include "symbgen.h"
#include "symb_remap.h"
#include "add_gaus_noise.h"
//#include "cmpxagn.h"
#include "mpskidealmod.h"
#include "qam_symbtowave.h"
#include "quadmod_bp.h"
#include "spec_analyzer.h"
#include "quad_carr_genie.h"
#include "quad_mixer_bp.h"
#include "integ_n_dump.h"
#include "qam_decoder.h"
#include "m_pskharddem.h"
#include "ser_ctr.h"
#include "siganchr.h"
#include "qam_theory.h"

//=========================================================

main()
{
#include "sim_preamble.cpp"

  //=========================================================================
  //  Misc special processing

  //-----------------------------------------------------
  // create plot file of theoretical PSD for QPSK signal

   int big_m;
   GET_INT_PARM( big_m );

   QamPsd(  big_m,
            0.0,  // carrier_freq
            1.0,  // symb_energy,
            1.0,  // symb_intvl,
            -4.0, // beg_freq,
            4.0,  // end_freq,
            801,  // num_pts,
            true, // plot_in_db,
            "msk_psd_theor.txt\0"); //out_filename

  //-----------------------------------------------------
  // create plot file of BER for ideal QPSK

  QamBer(  big_m,
            -10.0,  // beg_ebno,
            18.0, // end_ebno,
            401,  // num_pts,
            "theor_msk_ber.txt\0");  // out_filename
  //=========================================================================
  //  Allocate signals

  BYTE_SIGNAL(symb_vals);
  BYTE_SIGNAL(mapped_symb_vals);
  FLOAT_SIGNAL(i_baseband_wave);
  FLOAT_SIGNAL(q_baseband_wave);
  FLOAT_SIGNAL(modulated_signal);
  FLOAT_SIGNAL(noisy_sig);
  FLOAT_SIGNAL(i_recov_carrier_sig);
  FLOAT_SIGNAL(q_recov_carrier_sig);
  FLOAT_SIGNAL(i_demod_wave);
  FLOAT_SIGNAL(q_demod_wave);
  FLOAT_SIGNAL(i_samp_wave);
  FLOAT_SIGNAL(q_samp_wave);
  BIT_SIGNAL(symb_clock);
  COMPLEX_SIGNAL(cmpx_integ_sig);
  FLOAT_SIGNAL(power_meas_sig);
  FLOAT_SIGNAL(mag_signal);
  FLOAT_SIGNAL(phase_signal);
  BYTE_SIGNAL(symb_decis);
  BYTE_SIGNAL(symb_err_seq);

  //============================================================
  //  Construct, initialize and connect models

  SymbGener* symb_gen = new SymbGener( "symb_gen\0",
                                      CommSystem,
                                      symb_vals );

//  SymbRemapper* symb_mapper = new SymbRemapper( "symb_mapper\0",
//                                                CommSystem,
//                                                symb_vals,
//                                                mapped_symb_vals);

  QamSymbsToQuadWaves* qam_mod = new QamSymbsToQuadWaves( 
                                                "qam_mod\0",
                                                CommSystem,
                                                symb_vals,
                                                i_baseband_wave,
                                                q_baseband_wave,
                                                symb_clock);

  QuadBandpassModulator* quad_mod = new QuadBandpassModulator( "quad_mod\0",
                                                        CommSystem,
                                                        i_baseband_wave,
                                                        q_baseband_wave,
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

  QuadCarrierRecovGenie* carrier_recovery = new QuadCarrierRecovGenie( "carrier_recovery\0",
                                                                   CommSystem,
                                                                   i_recov_carrier_sig,
                                                                   q_recov_carrier_sig);
  
  QuadBandpassMixer* quad_dem = new QuadBandpassMixer( "quad_dem\0",
                                                   CommSystem,
                                                   noisy_sig,
                                                   i_recov_carrier_sig,
                                                   q_recov_carrier_sig,
                                                   i_demod_wave,
                                                   q_demod_wave );

  IntegrateAndDump* i_integrator = new IntegrateAndDump( "i_integrator\0",
                                           CommSystem,
                                           i_demod_wave,
                                           symb_clock,
                                           i_samp_wave);

  IntegrateAndDump* q_integrator = new IntegrateAndDump( "q_integrator\0",
                                           CommSystem,
                                           q_demod_wave,
                                           symb_clock,
                                           q_samp_wave);

  QamSymbolDecoder* qam_demod = new QamSymbolDecoder( 
                                                   "qam_demod\0",
                                                    CommSystem,
                                                    i_samp_wave,
                                                    q_samp_wave,
                                                    symb_decis);
  
  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                modulated_signal,
                                                0.015625, //samp_intvl
                                                4096 ); //block_size

  SerCounter* ser_ctr = new SerCounter( "ser_ctr\0",
                                        CommSystem,
                                        symb_vals,
                                        symb_decis,
                                        symb_err_seq );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
