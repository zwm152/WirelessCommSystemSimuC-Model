//
//  File = quad_mixer_bp.h
//

#ifndef _QUAD_MIXER_BP_H_
#define _QUAD_MIXER_BP_H_

#include "signal_T.h"
#include "psmodel.h"

class QuadBandpassMixer : public PracSimModel
{
public:
  QuadBandpassMixer(  char* instance_nam,
                    PracSimModel* outer_model,
                    Signal< float >* in_sig,
                    Signal< float >* i_ref_sig,
                    Signal< float >* q_ref_sig,
                    Signal< float >* i_out_sig,
                    Signal< float >* q_out_sig );

  ~QuadBandpassMixer(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  Signal< float > *I_Out_Sig;
  Signal< float > *Q_Out_Sig;
  Signal< float > *In_Sig;  
  Signal< float > *I_Ref_Sig;  
  Signal< float > *Q_Ref_Sig;  
};

#endif
