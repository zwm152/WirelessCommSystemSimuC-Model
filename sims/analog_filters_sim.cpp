//
//  File = filter_testbed_sim.cpp
//

#define SIM_NAME "Filters\0"
#define SIM_TITLE "Testbed for Butterworth Filter\0"

#include "global_stuff.h" 

#include "bitgen.h"
#include "bitwav.h"
#include "stepgen.h"
#include "noise_gen.h"
#include "swep_tone_xmtr.h"
#include "swep_tone_rcvr.h"
#include "siganchr.h"
#include "spec_analyzer.h"
#include "butt_filt_iir.h"
#include "butt_filt_intg.h"
#include "cheb_filt_iir.h"
#include "cheb_filt_intg.h"
#include "elip_filt_iir.h"
#include "elip_filt_intg.h"
#include "bess_filt_iir.h"
#include "ampl_to_db_pwr.h"
#include "fsk_theory.h"

//=========================================================

main()
{
#include "sim_preamble.cpp"
   int System_Mode_Switch_1;
   System_Mode_Switch_1 = ParmInput->GetIntParm("System_Mode_Switch_1\0");
  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  BIT_SIGNAL(bit_seq);
  BIT_SIGNAL(bit_clock);
  //FLOAT_SIGNAL(square_pulse_wave);
  FLOAT_SIGNAL(test_sig);
  FLOAT_SIGNAL(baseband_wave);
  FLOAT_SIGNAL(db_pwr_sig);
  FLOAT_SIGNAL(swept_rx_output);

  //============================================================
  //  Construct, initialize and connect models


  StepGener* step_gen = new StepGener( "step_gen\0",
                                      CommSystem,
                                      test_sig );

//  BitGener* bit_gen = new BitGener( "bit_gen\0",
//                                      CommSystem,
//                                      bit_seq );

//  BitsToWave* wave_gen = new BitsToWave( "wave_gen\0",
//                                           CommSystem,
//                                           bit_seq,
//                                           test_sig,
//                                           bit_clock );

//  GaussianNoiseGenerator *noise_gen = new GaussianNoiseGenerator( "noise_gen\0",
//                                                                  CommSystem,
//                                                                  test_sig);

//   Control<bool> *rx_enable = new Control<bool>(   "rx_enable\0",
//                                                   CommSystem);

//   Control<double> *swept_freq_ctl = new Control<double>( 
//                                                      "swept_freq_ctl\0",
//                                                      CommSystem);

//   Control<double> *block_phase_ctl = new Control<double>( 
//                                                      "block_phase_ctl\0",
//                                                      CommSystem);

//   SweptToneTransmitter *tone_xmtr = new SweptToneTransmitter( "tone_xmtr\0",
//                                        CommSystem,
//                                        test_sig,
//                                        rx_enable,
//                                        swept_freq_ctl,
//                                        block_phase_ctl );

  //SpectrumAnalyzer<float>* input_spec_an = 
  //                new SpectrumAnalyzer<float>( 
  //                                            "input_spec_an\0",
  //                                            CommSystem,
  //                                            test_sig );

  //EllipticalFilterByInteg* base_filt = new EllipticalFilterByInteg( "base_filt\0",
  //ButterworthFilterByInteg* base_filt = new ButterworthFilterByInteg( "base_filt\0",
  //ChebyshevFilterByInteg* base_filt = new ChebyshevFilterByInteg( "base_filt\0",
  //BesselFilterByInteg* base_filt = new BesselFilterByInteg( "base_filt\0",

   if(System_Mode_Switch_1==0)
   {
     ButterworthFilterByInteg* base_filt = new ButterworthFilterByInteg( "base_filt\0",
                                                        CommSystem,
                                                        test_sig,
                                                        baseband_wave);
   }
   else
   {
      ButterworthFilterByIir<float>* base_filt = new ButterworthFilterByIir<float>( "base_filt\0",
                                                        CommSystem,
                                                        test_sig,
                                                        baseband_wave);
      //SubordFilterTestShell<float>* base_filt = new SubordFilterTestShell<float>( "base_filt\0",
      //                                                  CommSystem,
      //                                                  test_sig,
      //                                                  baseband_wave);
   }

   AmplitudeToDbPower* db_converter = new AmplitudeToDbPower( "db_converter\0",
                                CommSystem,
                                baseband_wave,
                                db_pwr_sig);

  SpectrumAnalyzer<float>* output_spec_an = 
                  new SpectrumAnalyzer<float>( 
                                              "output_spec_an\0",
                                              CommSystem,
                                              baseband_wave );

//  SweptToneReceiver* tone_rcvr = new SweptToneReceiver( "tone_rcvr\0",
//                                                         CommSystem,
//                                                         baseband_wave,
//                                                         test_sig,
//                                                         swept_rx_output,
//                                                         rx_enable,
//                                                         swept_freq_ctl,
//                                                         block_phase_ctl);

  SignalAnchor* temp_anchor = new SignalAnchor( "temp_anchor\0",
                                                CommSystem,
                                                baseband_wave);
                                             //   0.0078125, //samp_rate
                                             //   4096 ); //block_size
//  SignalAnchor* temp_anchor_3 = new SignalAnchor( "temp_anchor_3\0",
//                                                CommSystem,
//                                                ref_seq,
//                                                1.0, //samp_rate
//                                                128 ); //block_size
  //=============================================================
  #include "sim_postamble.cpp"

  return 0;
}  
