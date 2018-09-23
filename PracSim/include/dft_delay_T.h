//
//  File = dft_delay_T.h
//

#ifndef _DFT_DELAY_T_H_
#define _DFT_DELAY_T_H_

#include "signal_T.h"
#include "psmodel.h"

template <class T>
class DftDelay : public PracSimModel
{
public:
  DftDelay<T>(  char* instance_nam,
                        PracSimModel* outer_model,
                        Signal< T >* in_sig,
                        Signal< T >* out_sig);

  ~DftDelay<T>(void);
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
   double K_Shift;

   double *Magnitude;
   double *Phase;
   double Dt_For_Fft;
   float Overlap_Save_Mem;

   char *Pt_Data_Fname;
   char *Resid_Data_Fname;
   char *Stretched_Data_Fname;

   ifstream *Pt_Data_File;
   int Num_Freq_Samps;

   double Ns_Exp;
   Signal< T > *In_Sig;
   Signal< T > *Out_Sig;

   int Num_Corr_Passes;
   std::complex<float> *Full_Buffer;
   std::complex<float> *Adj_Resp;
};

#endif
