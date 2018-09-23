//
//  File = nla_2_box_sim.cpp
//

#define SIM_NAME "NLA_2_Box\0"
#define SIM_TITLE "Complex Baseband Simulation for 2-Box NLA model\0"

#include "global_stuff.h" 

//#include "m_psk_theory.h"
#include "gausrand.h"
#include "symbgen.h"
#include "symb_remap.h"
#include "qam_symbtowave.h"
#include "add_gaus_noise.h"
#include "mpsk_symbtowave.h"
#include "quadmod.h"
#include "spec_analyzer.h"
#include "cmpx_iq_plot.h"
#include "m_pskharddem.h"
#include "ser_ctr.h"
#include "siganchr.h"
#include "nonlinear_amp.h"
#include "polar_freq_dom_filt.h"
#include "mpskoptimdem.h"
#include "cpxtoquad.h"
#include "quadcmpx.h"
#include "andf1fir.h"

//=========================================================

main()
{
#include "sim_preamble.cpp"

  //=========================================================================
  //  Misc special processing

  //=========================================================================
  //  Allocate signals

  BYTE_SIGNAL(symb_vals);
  BYTE_SIGNAL(mapped_symb_vals);
  FLOAT_SIGNAL(i_baseband_wave);
  FLOAT_SIGNAL(q_baseband_wave);
  COMPLEX_SIGNAL(modulated_signal);
  COMPLEX_SIGNAL(noisy_sig);
  COMPLEX_SIGNAL(nla_filt_sig);
  COMPLEX_SIGNAL(ampl_sig);
  COMPLEX_SIGNAL(filtered_rx_sig);
  BIT_SIGNAL(symb_clock);
  COMPLEX_SIGNAL(cmpx_integ_sig);
  FLOAT_SIGNAL(power_meas_sig);
  FLOAT_SIGNAL(mag_signal);
  FLOAT_SIGNAL(phase_signal);
  BYTE_SIGNAL(symb_decis);
  BYTE_SIGNAL(symb_err_seq);
  FLOAT_SIGNAL(i_demod_sig);
  FLOAT_SIGNAL(q_demod_sig);
  FLOAT_SIGNAL(i_filt_sig);
  FLOAT_SIGNAL(q_filt_sig);

  //============================================================
  //  Construct, initialize and connect models

  SymbGener* symb_gen = new SymbGener( "symb_gen\0",
                                      CommSystem,
                                      symb_vals );

  SymbRemapper* symb_mapper = new SymbRemapper( "symb_mapper\0",
                                                CommSystem,
                                                symb_vals,
                                                mapped_symb_vals);

  MpskSymbsToQuadWaves* m_psk_mod = new MpskSymbsToQuadWaves( 
                                                "m_psk_mod\0",
                                                CommSystem,
                                                mapped_symb_vals,
                                                i_baseband_wave,
                                                q_baseband_wave,
                                                symb_clock);

//    QamSymbsToQuadWaves* qam_mod = new QamSymbsToQuadWaves( 
//                                                "qam_mod\0",
//                                                CommSystem,
//                                                symb_vals,
//                                                i_baseband_wave,
//                                                q_baseband_wave,
//                                                symb_clock);

QuadratureModulator* quad_mod = new QuadratureModulator( "quad_mod\0",
                                                        CommSystem,
                                                        i_baseband_wave,
                                                        q_baseband_wave,
                                                        modulated_signal,
                                                        mag_signal,
                                                        phase_signal);

  AdditiveGaussianNoise<std::complex<float> >* agn_source = 
                  new AdditiveGaussianNoise<std::complex<float> >( 
                                                "agn_source\0",
                                                CommSystem,
                                                modulated_signal,
                                                noisy_sig,
                                                power_meas_sig);

   PolarFreqDomainFilter* nla_filt = new PolarFreqDomainFilter( "nla_filt\0",
                                                                CommSystem,
                                                                noisy_sig,
                                                                nla_filt_sig);

//  SpectrumAnalyzer<std::complex<float> >* spec_analyzer_2 = 
//                  new SpectrumAnalyzer<std::complex<float> >( 
//                                                  "spec_analyzer_2\0",
//                                                  CommSystem,
//                                                  nla_filt_sig );

   NonlinearAmplifier* nonlin_amp = new NonlinearAmplifier( "nonlin_amp\0",
                                                            CommSystem,
                                                            nla_filt_sig,
                                                            ampl_sig);

//  AdditiveGaussianNoise<std::complex<float> >* agn_source = 
//                  new AdditiveGaussianNoise<std::complex<float> >( 
//                                                "agn_source\0",
//                                                CommSystem,
//                                                nla_filt_sig,
//                                                noisy_sig,
//                                                power_meas_sig);

  SpectrumAnalyzer<std::complex<float> >* spec_analyzer_1 = 
                  new SpectrumAnalyzer<std::complex<float> >( 
                                                  "spec_analyzer_1\0",
                                                  CommSystem,
                                                  noisy_sig );


CmpxToQuadrature* quad_demod = new CmpxToQuadrature( "quad_demod\0",
                                                      CommSystem,
                                                      ampl_sig,
                                                      i_demod_sig,
                                                      q_demod_sig);

   AnlgDirectFormFir* i_rx_filt = new AnlgDirectFormFir( "i_rx_filt\0",
                                                          CommSystem,
                                                          i_demod_sig,
                                                          i_filt_sig );

   AnlgDirectFormFir* q_rx_filt = new AnlgDirectFormFir( "q_rx_filt\0",
                                                          CommSystem,
                                                          q_demod_sig,
                                                          q_filt_sig );

   QuadratureToComplex* i_q_combiner = new QuadratureToComplex( "i_q_combiner\0",
                                                                  CommSystem,
                                                                  i_filt_sig,
                                                                  q_filt_sig,
                                                                  filtered_rx_sig);

  SpectrumAnalyzer<std::complex<float> >* spec_analyzer_3 = 
                  new SpectrumAnalyzer<std::complex<float> >( 
                                                  "spec_analyzer_3\0",
                                                  CommSystem,
                                                  filtered_rx_sig );

  MpskOptimalDemod* m_psk_dem = new MpskOptimalDemod( "m_psk_dem\0",
                                                    CommSystem,
                                                    noisy_sig,
                                                    symb_clock,
                                                    symb_decis);
  
  CmpxIqPlot* iq_plot = new CmpxIqPlot( "iq_plot\0",
                                        CommSystem,
                                        noisy_sig,
                                        symb_clock);
  
  CmpxIqPlot* filt_iq_plot = new CmpxIqPlot( "filt_iq_plot\0",
                                        CommSystem,
                                        filtered_rx_sig,
                                        symb_clock);
  
//  CmpxIqPlot* amp_iq_plot = new CmpxIqPlot( "amp_iq_plot\0",
//                                        CommSystem,
//                                        ampl_sig,
//                                        symb_clock);
  
  SignalAnchor* temp_anchor_1 = new SignalAnchor( "temp_anchor_1\0",
                                                CommSystem,
                                                modulated_signal,
                                                0.0625, //samp_intvl
                                                4080 ); //block_size

  SerCounter* ser_ctr = new SerCounter( "ser_ctr\0",
                                        CommSystem,
                                        mapped_symb_vals,
                                        symb_decis,
                                        symb_err_seq );

  //=============================================================
  #include "sim_postamble.cpp"
  return 0;
}  
