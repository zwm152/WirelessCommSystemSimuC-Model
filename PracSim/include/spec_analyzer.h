//
//  File = spec_analyzer.h
//

#ifndef _SPECANAL_H_
#define _SPECANAL_H_

#include "psmodel.h"
#include "signal_T.h"
#include "spect_calc_kinds.h"
#include "samp_spect_util.h"
#include "spec_estim.h"
#include "gen_win.h"

template <class T>
class SpectrumAnalyzer : public PracSimModel
{
public:
  SpectrumAnalyzer( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~SpectrumAnalyzer(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<T> *In_Sig;
  double Samp_Intvl;
  //double *Sample_Spectrum;
  T *Time_Seg;
  double *Psd_Est;
  int Num_Segs_To_Avg;
  int Segs_In_Est;
  int Fft_Len;
  int Hold_Off;
  int Seg_Len;
  int Samps_Needed;
  char *Psd_File_Name;
  ofstream *Psd_File;
  bool Halt_When_Completed;
  bool Processing_Completed;
  bool Output_In_Decibels;
  bool Plot_Two_Sided;
  bool Plot_Relative_To_Peak;
  double Delta_F;
  double Norm_Factor;
  double Freq_Norm_Factor;
  GenericWindow* Data_Window;
  SpectrumEstimator<T> *Spec_Estim;
  //void (*Spectrum_Calc)( T* time_signal,
  //                   double* psd_estimate,
  //                   int num_samps,
  //                   int fft_len,
  //                   double samp_intvl );
   KIND_OF_SPECT_CALC_T Kind_Of_Spec_Estim;

};

#endif //_SPECANAL_H_
