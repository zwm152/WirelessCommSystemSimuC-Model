//
//  File = butt_filt_intg.h
//

#ifndef _BUTT_FILT_INTG_H_
#define _BUTT_FILT_INTG_H_

#include "anlg_filt_intg.h"
#include "psmodel.h"

class ButterworthFilterByInteg : public AnalogFilterByInteg
{
public:
  ButterworthFilterByInteg( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<float> *in_sig,
                   Signal<float> *out_sig );

  ButterworthFilterByInteg( char *instance_name,
                    int kind_of_interface,
                    int filter_band_config,
                    int filter_order,
                    float upper_passband_edge,
                    float lower_passband_edge,
                    bool bypass_enabled);
  ~ButterworthFilterByInteg(void);
  //virtual void Initialize(void);
  
};

#endif
