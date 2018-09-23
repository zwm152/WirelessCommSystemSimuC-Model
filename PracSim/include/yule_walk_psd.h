//
//  File = yule_walk_psd.h
//

#ifndef _YULE_WALK_PSD_H_
#define _YULE_WALK_PSD_H_

#include "psmodel.h"
#include "signal_T.h"

class YuleWalkerPsdEstim : public PracSimModel
{
public:
  YuleWalkerPsdEstim( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<float>* in_sig );

  ~YuleWalkerPsdEstim(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal<float> *In_Sig;
  std::complex<double> *Freq_Seg;
  double *Sample_Spectrum;
  double Samp_Intvl;
  double *Time_Seg;
  double *Psd_Est;
  int Num_Freq_Pts;
  int Segs_In_Est;
  int Ar_Order;
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

#endif
