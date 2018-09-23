//
//  File = butt_filt_iir.h
//

#ifndef _BUTT_FILT_IIR_H_
#define _BUTT_FILT_IIR_H_

#include "anlg_filt_iir.h"
#include "psmodel.h"

template <class T>
class ButterworthFilterByIir : public AnalogFilterByIir<T>
{
public:
  ButterworthFilterByIir( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<T> *in_sig,
                   Signal<T> *out_sig );

   ButterworthFilterByIir( char *instance_name,
                           PracSimModel *outer_model );

//  ButterworthFilterByIir( char *instance_name,
//                    int kind_of_interface,
//                    int filter_band_config,
//                    int filter_order,
//                    float upper_passband_edge,
//                    float lower_passband_edge,
//                    bool bypass_enabled);
  ~ButterworthFilterByIir(void);
  //virtual void Initialize(void);
  
};

#endif
