//
//  File = qam_symbtowave.h
//

#ifndef _QAM_SYMBTOWAVE_H_
#define _QAM_SYMBTOWAVE_H_

#include "signal_T.h"
#include "psmodel.h"

class QamSymbsToQuadWaves : public PracSimModel
{
public:
  QamSymbsToQuadWaves(  char* instance_nam,
                  PracSimModel* outer_model,
                  Signal<byte_t>* in_symb_seq,
                  Signal<float>* i_out_sig,
                  Signal<float>* q_out_sig,
                  Signal<bit_t>* symb_clock_out );

  ~QamSymbsToQuadWaves(void);
  void Initialize(void);
  int Execute(void);

private:
  int In_Block_Size;
  int Out_Block_Size;
  int Bits_Per_Symb;
  int Num_Symb_Rows;
  int Samps_Per_Symb;
  double Symb_Duration;
  float *I_Compon;
  float *Q_Compon;
  Signal<byte_t> *In_Symb_Seq;
  Signal<float> *I_Out_Sig;
  Signal<float> *Q_Out_Sig;
  Signal<bit_t> *Symb_Clock_Out;
  Signal< std::complex<float> > *Cmpx_Out_Sig;  
};

#endif
