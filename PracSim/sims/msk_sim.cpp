//
//  File = msk_sim.cpp
//

#define SIM_NAME "MskSim\0"
#define SIM_TITLE "Complex Baseband Simulation for MSK\0"

#include "global_stuff.h" 

#include "gausrand.h"
#include "bitgen.h"
#include "bitwav.h"
#include "mskmod.h"
#include "add_gaus_noise.h"
#include "spec_analyzer.h"
#include "phase_genie.h"
#include "quaddem.h"
#include "qpskdem.h"
#include "msk_genie.h"
#include "integ_dump_slice.h"
#include "discrete_delay_T.h"
#include "ber_ctr.h"
#include "siganchr.h"
#include "msk_theory.h"
#include "sigstuff.h"
#include "parmfile.h"
#include "model_graph.h"
#include "sigplot.h"
#include "reports.h"
#include "exec.h"
#include "psstream.h"
   

//#ifdef _DEBUG
// ofstream DebugFile("MskModulator.dbg", ios::out);
//#endif

//=========================================================
//  Global Stuff

//ParmFile ParmInput("MskModulator.dat");
//ofstream LongReport;
//ofstream ShortReport;
//PracSimStream DetailedResults;
//PracSimStream BasicResults;
//PracSimStream ErrorStream;

//PracSimModel *CommSystem;
//ActiveSystemGraph ActSystGraph;
//Executive Exec;
//PracSimModel *PrevModelConstr;
//PracSimModel *ActiveModel;
//SignalPlotter SigPlot;
//int PassNumber;
//int MaxPassNumber;
//int EnclaveNumber;

//=========================================================

main()
{
#include "sim_preamble.cpp"

  //=========================================================================
  //  Misc special processing

  //-----------------------------------------------------
  // create plot file of theoretical PSD for MSK signal

  MskPsd(  0.0,  // carrier_freq
            1.0,  // symb_energy,
            1.0,  // symb_intvl,
            -4.0, // beg_freq,
            4.0,  // end_freq,
            801,  // num_pts,
            true, // plot_in_db,
            "msk_psd_theor.txt\0"); //out_filename

  //-----------------------------------------------------
  // create plot file of BER for ideal MSK

  MskBer(  -10.0,  // beg_ebno,
            18.0, // end_ebno,
            401,  // num_pts,
            "theor_msk_ber.txt\0");  // out_filename
  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(i_bits);
  BIT_SIGNAL(q_bits);
  BIT_SIGNAL(q_bits_delayed);
  FLOAT_SIGNAL(i_baseband_wave);
  FLOAT_SIGNAL(q_baseband_wave);
  BIT_SIGNAL(i_bit_clock);
  BIT_SIGNAL(q_bit_clock);
  FLOAT_SIGNAL(i_filtered_wave);
  FLOAT_SIGNAL(q_filtered_wave);
  COMPLEX_SIGNAL(modulated_signal);
  COMPLEX_SIGNAL(filtered_sig);
  COMPLEX_SIGNAL(noisy_sig);
  COMPLEX_SIGNAL(phase_ref_sig);
  FLOAT_SIGNAL(power_meas_sig);
  FLOAT_SIGNAL(mag_signal);
  FLOAT_SIGNAL(phase_signal);
  FLOAT_SIGNAL(i_demod_wave);
  FLOAT_SIGNAL(q_demod_wave);
  FLOAT_SIGNAL(i_corr_sig);
  FLOAT_SIGNAL(q_corr_sig);
  FLOAT_SIGNAL(i_filt_demod_wave);
  FLOAT_SIGNAL(q_filt_demod_wave);
  FLOAT_SIGNAL(i_samp_demod_wave);
  FLOAT_SIGNAL(q_samp_demod_wave);
  BIT_SIGNAL(i_bit_decis);
  BIT_SIGNAL(q_bit_decis);

  //============================================================
  //  Construct, initialize and connect models

  EnclaveNumber = 0;

  BitGener* i_bit_gen = new BitGener( "i_bit_gen\0",
                                      CommSystem,
                                      i_bits );

  BitGener* q_bit_gen = new BitGener( "q_bit_gen\0",
                                      CommSystem,
                                      q_bits );

  DiscreteDelay<bit_t>* q_bit_delay = new DiscreteDelay<bit_t>( 
                                "q_bit_delay\0",
                                CommSystem,
                                q_bits,
                                q_bits_delayed);

  BitsToWave* i_wave_gen = new BitsToWave( "i_wave_gen\0",
                                           CommSystem,
                                           i_bits,
                                           i_baseband_wave,
                                           i_bit_clock );

  BitsToWave* q_wave_gen = new BitsToWave( "q_wave_gen\0",
                                           CommSystem,
                                           q_bits,
                                           q_baseband_wave,
                                           q_bit_clock );

  MskModulator* msk_mod = new MskModulator( "msk_mod\0",
                                            CommSystem,
                                            i_baseband_wave,
                                            q_baseband_wave,
                                            modulated_signal,
                                            mag_signal,
                                            phase_signal );
  
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

   PhaseRecoveryGenie* phase_genie = new PhaseRecoveryGenie( "phase_genie\0",
                                                            CommSystem,
                                                            phase_ref_sig);

   QuadratureDemod* quad_demod = new QuadratureDemod( "quad_demod\0",
                                                      CommSystem,
                                                      noisy_sig,
                                                      phase_ref_sig,
                                                      i_demod_wave,
                                                      q_demod_wave);

//  QpskDemodulator* qpsk_dem = new QpskDemodulator( "qpsk_dem\0",
//                                                    CommSystem,
//                                                    noisy_sig,
//                                                    i_demod_wave,
//                                                    q_demod_wave);

  SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                i_baseband_wave,
                                                q_baseband_wave,
                                                0.0625, //samp_intvl
                                                4096 ); //block_size

   MskShapeGenie* msk_genie = new MskShapeGenie( "msk_genie\0",
                                                   CommSystem,
                                                   i_corr_sig,
                                                   q_corr_sig);

  IntegrateDumpAndSlice* i_bit_slicer = new IntegrateDumpAndSlice( "i_bit_slicer\0",
                                          CommSystem,
                                          i_demod_wave,
                                          //i_corr_sig,
                                          i_bit_clock,
                                          i_samp_demod_wave,
                                          i_bit_decis );

  BerCounter* i_ber_ctr = new BerCounter( "i_ber_ctr\0",
                                        CommSystem,
                                        i_bits,
                                        i_bit_decis );

  IntegrateDumpAndSlice* q_bit_slicer = new IntegrateDumpAndSlice( "q_bit_slicer\0",
                                          CommSystem,
                                          q_demod_wave,
                                          //q_corr_sig,
                                          q_bit_clock,
                                          q_samp_demod_wave,
                                          q_bit_decis );

  BerCounter* q_ber_ctr = new BerCounter( "q_ber_ctr\0",
                                        CommSystem,
                                        q_bits_delayed,
                                        q_bit_decis );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
