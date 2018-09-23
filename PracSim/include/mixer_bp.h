//
//  File = mixer_bp.h
//

#ifndef _MIXER_BP_H_
#define _MIXER_BP_H_

#include "signal_T.h"
#include "psmodel.h"

class BandpassMixer : public PracSimModel
{
public:
  BandpassMixer(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< float >* in_sig_1,
                    Signal< float >* in_sig_2,
                    Signal< float >* out_sig );

  ~BandpassMixer(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal< float > *Out_Sig;
  Signal< float > *In_Sig_1;  
  Signal< float > *In_Sig_2;  
};

#endif
