//
//  File = polar_freq_dom_filt.h
//

#ifndef _POLAR_FREQ_DOM_FILT_H_
#define _POLAR_FREQ_DOM_FILT_H_

#include "signal_T.h"
#include "psmodel.h"

class PolarFreqDomainFilter : public PracSimModel
{
public:
  PolarFreqDomainFilter(  char* instance_nam,
                        PracSimModel* outer_model,
                        Signal< std::complex<float> >* in_sig,
                        Signal< std::complex<float> >* out_sig);

  ~PolarFreqDomainFilter(void);
  void Initialize(void);
  int Execute(void);

private:
   int Proc_Block_Size;
   int Block_Size;
   double Samp_Rate;

   bool Bias_Removal_Enab;
   bool Decomp_Enab;
   int Skirt_Type;
   bool Group_Delay_Enab;
   bool Output_Decomp_Enab;
   int Amp_Decomp_Order;
   int Phase_Decomp_Order;
   int Fft_Size;
   int Num_Saved_Samps;
   bool Bypass_Enabled;

   double *Magnitude;
   double *Phase;
   double Dt_For_Fft;
   float Overlap_Save_Mem;

   char *Magnitude_Data_Fname;
   char *Phase_Data_Fname;
   char *Resid_Data_Fname;
   char *Stretched_Data_Fname;
   float *Raw_Magnitude_Resp;
   float *Raw_Phase_Resp;
   float *Freqs_For_Magnitude;
   float *Freqs_For_Phase;

   ifstream *Magnitude_Data_File;
   ifstream *Phase_Data_File;
   int Num_Mag_Samps;
   int Num_Phase_Samps;
   double Mag_Freq_Scaling_Factor;
   double Phase_Freq_Scaling_Factor;

   double Ns_Exp;
   Signal< std::complex<float> > *In_Sig;
   Signal< std::complex<float> > *Out_Sig;

   int Num_Corr_Passes;
   std::complex<float> *Full_Buffer;
   //std::complex<float> *Adj_Resp;
   float *Mag_Resp;
   float *Phase_Resp;
};

#endif
