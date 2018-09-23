//
//  File = bart_pdgm_wind.h
//

#ifndef _BART_PDGM_WIND_H_
#define _BART_PDGM_WIND_H_

#include "psmodel.h"
#include "signal_T.h"
#include "gen_win.h"
#include "window_shapes.h"

template <class T>
class BartlettPeriodogramWindowed : public PracSimModel
{
public:
  BartlettPeriodogramWindowed( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~BartlettPeriodogramWindowed(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<T> *In_Sig;
  std::complex<double> *Freq_Seg;
  double *Sample_Spectrum;
  double Samp_Intvl;
  T *Time_Seg;
  T *Win_Time_Seg;
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
  bool Using_Window;
  GenericWindow* Data_Window;
  double *Window_Taps;
  WINDOW_SHAPE_T Window_Shape;
  double Delta_F;
  double Norm_Factor;
  double Freq_Norm_Factor;
  double Window_Power;
};
#endif //_BART_PDGM_WIND_H_
