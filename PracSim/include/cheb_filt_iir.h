//
//  File = cheb_filt_iir.h
//

#ifndef _CHEB_FILT_IIR_H_
#define _CHEB_FILT_IIR_H_

#include "anlg_filt_iir.h"
#include "psmodel.h"

template <class T>
class ChebyshevFilterByIir : public AnalogFilterByIir<T>
{
public:
  ChebyshevFilterByIir( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<T> *in_sig,
                   Signal<T> *out_sig );

//  ChebyshevFilter( char *instance_name,
//                    int kind_of_interface,
//                    int filter_band_config,
//                    int filter_order,
//                    float upper_passband_edge,
//                    float lower_passband_edge,
//                    bool bypass_enabled);
  ~ChebyshevFilterByIir(void);
  //virtual void Initialize(void);
private:
  double Passband_Ripple_In_Db;
  bool Bw_Is_Ripple_Bw;
  
};

#endif
