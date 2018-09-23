//
//  File = cheb_filt_intg.h
//

#ifndef _CHEB_FILT_INTG_H_
#define _CHEB_FILT_INTG_H_

#include "signal_T.h"
#include "anlg_filt_intg.h"
#include "psmodel.h"

class ChebyshevFilterByInteg : public AnalogFilterByInteg
{
public:
  ChebyshevFilterByInteg( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<float> *in_sig,
                   Signal<float> *out_sig );

//  ChebyshevFilter( char *instance_name,
//                    int kind_of_interface,
//                    int filter_band_config,
//                    int filter_order,
//                    float upper_passband_edge,
//                    float lower_passband_edge,
//                    bool bypass_enabled);
  ~ChebyshevFilterByInteg(void);
  //virtual void Initialize(void);
private:
  double Passband_Ripple_In_Db;
  bool Bw_Is_Ripple_Bw;
  
};

#endif
