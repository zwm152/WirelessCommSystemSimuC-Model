//
//  File = correlator.h
//

#ifndef _CORRELATOR_H_
#define _CORRELATOR_H_

//#include "intsig.h"
#include "signal_T.h"
#include "control_T.h"
#include "psmodel.h"

class SweptToneReceiver : public PracSimModel
{
public:
  SweptToneReceiver( char* instance_name,
                  PracSimModel* outer_model,
                  Signal<float>* in_sig,
                  Signal<float>* ref_sig,
                  Signal<float>* out_sig,
                  Control<bool>* rx_enable,
                  Control<double>* inst_freq_ctl,
                  Control<double>* block_phase_ctl );

  ~SweptToneReceiver(void);
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

  double Ns_Exp;
  Signal<float> *In_Sig;
  Signal<float> *Out_Sig;
  Signal<float> *Ref_Sig;
  Control<double> *Inst_Freq_Ctl;
  Control<double> *Block_Phase_Ctl;

  int Num_Phases;
  double Max_Phase_Shift;
  char *Response_File_Name;
  int Max_Num_Freqs;
  int Num_Freqs;
  Control<bool>* Rx_Enable;
  bool Tone_Received;
  ofstream *Response_File;
  double *Freqs;
  double *Magnitudes;
  double *Phases;
  double Prev_Phase;
};

#endif
