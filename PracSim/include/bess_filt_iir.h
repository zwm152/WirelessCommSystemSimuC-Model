//
//  File = bess_filt_iir.h
//

#ifndef _BESS_FILT_IIR_H_
#define _BESS_FILT_IIR_H_

#include "anlg_filt_iir.h"
#include "psmodel.h"

template <class T>
class BesselFilterByIir : public AnalogFilterByIir<T>
{
public:
  BesselFilterByIir( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<T> *in_sig,
                   Signal<T> *out_sig );

//  BesselFilterByIir( char *instance_name,
//                    int kind_of_interface,
//                    int filter_band_config,
//                    int filter_order,
//                    float upper_passband_edge,
//                    float lower_passband_edge,
//                    bool bypass_enabled);
  ~BesselFilterByIir(void);
  //virtual void Initialize(void);
private:
  bool Delay_Norm_Enabled;
  
};

#endif
