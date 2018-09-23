//
//  File = qam_decoder.h
//

#ifndef _QAM_DECODER_H_
#define _QAM_DECODER_H_

#include "signal_T.h"
#include "psmodel.h"

class QamSymbolDecoder : public PracSimModel
{
public:
  QamSymbolDecoder(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal<float>* i_in_sig,
                    Signal<float>* q_in_sig,
                    Signal< byte_t >* out_sig );

  ~QamSymbolDecoder(void);
  void Initialize(void);
  int Execute(void);

private:
  double Out_Samp_Intvl;
  int Block_Size;
  Signal< byte_t > *Out_Sig;
  Signal<float> *I_In_Sig;  
  Signal<float> *Q_In_Sig;  
  int Bits_Per_Symb;
  int Num_Symb_Rows;
  double *I_Boundary;
  double *Q_Boundary;
};

#endif
