//
//  File = welch_pdgm.h
//

#ifndef _WELCH_PDGM_H_
#define _WELCH_PDGM_H_

#include "psmodel.h"
#include "signal_T.h"
#include "gen_win.h"
#include "window_shapes.h"
//#include "spect_calc_kinds.h"
//#include "samp_spect.h"
//#include "compress.h"
//#include "dirform1.h"
//using namespace std;

template <class T>
class WelchPeriodogram : public PracSimModel
{
public:
  WelchPeriodogram( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~WelchPeriodogram(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<T> *In_Sig;
  std::complex<double> *Freq_Seg;
  double *Sample_Spectrum;
  double Samp_Intvl;
  //double *Sample_Spectrum;
  T *Time_Seg;
  T *Overlap_Seg;
  T *Win_Time_Seg;
  double *Psd_Est;
  int Num_Segs_To_Avg;
  int Segs_In_Est;
  int Fft_Len;
  int Hold_Off;
  int Seg_Len;
  int Samps_Needed;
  int Shift_Between_Segs;
  int Overlap_Len;
  char *Psd_File_Name;
  ofstream *Psd_File;
  bool Halt_When_Completed;
  bool Processing_Completed;
  bool Output_In_Decibels;
  bool Plot_Two_Sided;
  bool Using_Window;
  GenericWindow* Data_Window;
  double *Window_Taps;
  WINDOW_SHAPE_T Window_Shape;
  double Delta_F;
  double Norm_Factor;
  double Freq_Norm_Factor;
  double Window_Power;
  //void (*Spectrum_Calc)( T* time_signal,
  //                   double* psd_estimate,
  //                   int num_samps,
  //                   int fft_len,
  //                   double samp_intvl );
//   KIND_OF_SPECT_CALC_T Kind_Of_Spec_Estim;

};

#endif //_BART_PDGM_WIND_H_
