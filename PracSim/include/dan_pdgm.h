//
//  File = dan_pdgm.h
//

#ifndef _DAN_PDGM_H_
#define _DAN_PDGM_H_

#include "psmodel.h"
#include "signal_T.h"
#include "spect_calc_kinds.h"
#include "samp_spect.h"
//#include "compress.h"
//#include "dirform1.h"
//using namespace std;

template <class T>
class DaniellPeriodogram : public PracSimModel
{
public:
  DaniellPeriodogram( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~DaniellPeriodogram(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<T> *In_Sig;
  std::complex<double> *Freq_Seg;
  double *Dan_Pdgm;
  double Samp_Intvl;
  //double *Sample_Spectrum;
  T *Time_Seg;
  double *Psd_Est;
  int Big_P;
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

#endif //_DAN_PDGM_H_
