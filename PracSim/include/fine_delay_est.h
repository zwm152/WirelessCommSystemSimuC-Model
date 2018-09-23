//
//  File = fine_delay_est.h
//

#ifndef _FINE_DELAY_EST_H_
#define _FINE_DELAY_EST_H_

//#include "intsig.h"
#include "signal_T.h"
#include "control_T.h"
#include "psmodel.h"
#include "aux_sig_buf.h"

class FineDelayEstimator : public PracSimModel
{
public:
  FineDelayEstimator( char* instance_name,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal<float>* ref_sig,
                  Control<bool>* estim_enabled_cntl, //input
                  Control<bool>* dly_est_is_valid_cntl,
                  Control<float>* estimated_delay_cntl);//,
                  //Control<int>* samps_delay_at_max_corr );

  ~FineDelayEstimator(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  double Samp_Intvl;

  // correlation array - contains In_Sig
  // values and is padded out with zeros
  std::complex<float> *X;

  // correlation array - contains Ref_Sig
  // values and is padded out with zeros
  std::complex<float> *Y;

  // an array of zeros used to quickly
  // pad the ends of X and Y with zeros
  std::complex<float> *Zero_Array;

  std::complex<float> Max_Corr;
  float Max_Corr_Angle;
  float Max_Corr_Time;

  int Full_Corr_Size;
//  int Search_Window_Beg;
//  int Search_Window_End;
//  int Neg_Window_Beg;
//  int Neg_Window_End;
//  int Pos_Window_Beg;
//  int Pos_Window_End;
  bool Invert_Input_Sig_Enab;
//  bool Limited_Search_Window_Enab;
//  int Smoothing_Sidelobe_Len;
  std::complex<float> *Diff_Response;

  AuxSignalBuffer<float> *In_Sig_Buf;
  AuxSignalBuffer<float> *Ref_Sig_Buf;

  int Size_Of_FComplex;

  double Ns_Exp;
  Signal<float> *In_Sig;
 // Signal<float> *Out_Sig;
  Signal<float> *Ref_Sig;
  Control<float> *Estimated_Delay_Cntl;
  //Control<int> *Samps_Delay_At_Max_Corr;
  //Control<float> *Max_Corr_Out;
  Control<bool> *Dly_Est_Is_Valid_Cntl;
  Control<bool> *Estim_Enabled_Cntl;
  
  

  int Num_Corr_Passes;
  int Corr_Pass_Count;
  ofstream *Spectrum_File;
};

#endif
