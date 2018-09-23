//
//  File = contindelay_sim.cpp
//
#define SIM_NAME "CoarseDelayEst\0"
#define SIM_TITLE "Coarse Delay Estimation Testbed\0"

#include "global_stuff.h"

//=====================
// model includes
#include "sinc_wave.h"
#include "bitgen.h"
#include "basewav.h"
#include "butt_filt_iir.h"
//#include "bitwav.h"
#include "ramp_gen.h"
#include "contin_delay_tester.h"
#include "contin_delay2_T.h"
#include "coarse_delay_est.h"
#include "discrete_delay_T.h"
#include "dft_delay_T.h"
#include "siganchr.h"
//=========================================================

main()
{
   #include "sim_preamble.cpp"
  //=========================================================================
  //  Misc special processing

   SincWaveform(  4, //num_sidelobes
                 50, //samps_per_hump
                 "sinc_wave.txt\0");//out_filename


  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);
  BIT_SIGNAL(bit_clock);
  FLOAT_SIGNAL(binary_wave);
  FLOAT_SIGNAL(filt_wave);
  FLOAT_SIGNAL(test_sig);
  FLOAT_SIGNAL(test_sig_2);
  FLOAT_SIGNAL(delayed_test_sig);
  FLOAT_SIGNAL(coarse_dly_ref_sig);
  FLOAT_SIGNAL(corr_out_sig);

  FLOAT_SIGNAL(baseband_wave);

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

  ButterworthFilterByIir<float>* base_filt = new ButterworthFilterByIir<float>( "base_filt\0",
                                                        CommSystem,
                                                        binary_wave,
                                                        filt_wave);

  //Control<bool>* delay_chg_enab = new Control<bool>( "delay_chg_enab\0",
  //                                                 CommSystem );

  //Control<float>* dynam_float_dly = new Control<float>( "dynam_float_dly\0",
  //                                                 CommSystem );
  //ContinuousDelayTester< float >* dyn_delay_knob = 
  //                          new ContinuousDelayTester< float >( 
  //                                                "dyn_delay_knob\0",
  //                                                CommSystem,
  //                                                filt_wave,
  //                                                test_sig_2,
  //                                                dynam_float_dly,
  //                                                delay_chg_enab );
  
  ContinuousDelay2< float >* delay_2 = new ContinuousDelay2< float >( 
                                                "delay_2\0",
                                                CommSystem,
                                                filt_wave,
                                                delayed_test_sig);

  Control<float>* delay_at_max_corr = new Control<float>( "delay_at_max_corr\0",
                                                          CommSystem);

   Control<int>* samps_delay_est_cntl = new Control<int>( "samps_delay_est_cntl\0",
                                                            CommSystem);

   Control<bool>* dly_est_is_valid_cntl = new Control<bool>( "dly_est_is_valid_cntl\0",
                                                            CommSystem);

   CoarseDelayEstimator* correlator = new CoarseDelayEstimator(  "correlator\0",
                                                    CommSystem,
                                                    delayed_test_sig,
                                                    filt_wave,
                                                    corr_out_sig,
                                                    dly_est_is_valid_cntl,
                                                    delay_at_max_corr,
                                                    samps_delay_est_cntl);

   DiscreteDelay< float >* coarse_dly_adj = new DiscreteDelay<float>(  "coarse_dly_adj\0",
                                                               CommSystem,
                                                               filt_wave,
                                                               coarse_dly_ref_sig,
                                                               samps_delay_est_cntl,
                                                               dly_est_is_valid_cntl);

//  SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
//                                                CommSystem,
//                                                filt_wave,
//                                                0.0078125, //samp_intvl
//                                                4096 ); //block_size
//  SignalAnchor* temp_anchor_3 = new SignalAnchor( "temp_anchor_3\0",
//                                                CommSystem,
//                                                ref_seq,
//                                                1.0, //samp_rate
//                                                128 ); //block_size
  //=============================================================

  #include "sim_postamble.cpp"
  return 0;
}  
