//
//  File = bart_pdgm.h
//

#ifndef _BART_PDGM_H_
#define _BART_PDGM_H_

#include "psmodel.h"
#include "signal_T.h"

template <class T>
class BartlettPeriodogram : public PracSimModel
{
public:
  BartlettPeriodogram( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~BartlettPeriodogram(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<T> *In_Sig;
  std::complex<double> *Freq_Seg;
  double *Sample_Spectrum;
  double Samp_Intvl;
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

};

#endif //_BART_PDGM_H_
