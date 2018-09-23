//
//  File = samp_spect.h
//

#ifndef _SAMP_SPECT_H_
#define _SAMP_SPECT_H_

#include "psmodel.h"
#include "signal_T.h"
#include "spect_calc_kinds.h"
//#include "samp_spect.h"
//#include "compress.h"
//#include "dirform1.h"
//using namespace std;

template <class T>
class SampleSpectrum : public PracSimModel
{
public:
  SampleSpectrum( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~SampleSpectrum(void);
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
  double Delta_F;
  double Norm_Factor;
  double Freq_Norm_Factor;
  //void (*Spectrum_Calc)( T* time_signal,
  //                   double* psd_estimate,
  //                   int num_samps,
  //                   int fft_len,
  //                   double samp_intvl );
   KIND_OF_SPECT_CALC_T Kind_Of_Spec_Estim;

};

#endif //_SPECANAL_H_
