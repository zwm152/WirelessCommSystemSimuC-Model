//
//  File = disc_auto_corr.h
//

#ifndef _DISC_AUTO_CORR_H_
#define _DISC_AUTO_CORR_H_

#include "signal_T.h"
#include "psmodel.h"

class DiscreteAutoCorr : public PracSimModel
{
public:
  DiscreteAutoCorr( char* instance_name,
                  PracSimModel* outer_model,
                  Signal<byte_t>* in_sig );

  ~DiscreteAutoCorr(void);
  void Initialize(void);
  int Execute(void);

private:
  int Proc_Block_Size;
  double Samp_Intvl;

  // correlation array - contains In_Sig
  // values and is padded out with zeros
  //std::complex<float> *X;

  // correlation array - contains Ref_Sig
  // values and is padded out with zeros
  //std::complex<float> *Y;

  // an array of zeros used to quickly
  // pad the ends of X and Y with zeros
  //std::complex<float> *Zero_Array;

  //std::complex<float> Max_Corr;
  //float Max_Corr_Angle;
  //float Max_Corr_Time;

  //int Full_Corr_Size;
  //int Search_Window_Beg;
  //int Search_Window_End;
  //int Neg_Window_Beg;
  //int Neg_Window_End;
  //int Pos_Window_Beg;
  //int Pos_Window_End;
  //bool Invert_Input_Sig_Enab;
  //bool Limited_Search_Window_Enab;

  //int Size_Of_FComplex;

  //double Ns_Exp;
  Signal<byte_t> *In_Sig;

  int Num_Corr_Passes;
  float *Corr_Buf;
};

#endif
