//
//  File = elip_filt_iir.h
//

#ifndef _ELIP_FILT_IIR_H_
#define _ELIP_FILT_IIR_H_

#include "anlg_filt_iir.h"
#include "psmodel.h"

template < class T>
class EllipticalFilterByIir : public AnalogFilterByIir<T>
{
public:
  EllipticalFilterByIir( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<T> *in_sig,
                   Signal<T> *out_sig );

//  EllipticalFilter( char *instance_name,
//                    int kind_of_interface,
//                    int filter_band_config,
//                    int filter_order,
//                    float upper_passband_edge,
//                    float lower_passband_edge,
//                    bool bypass_enabled);
  ~EllipticalFilterByIir(void);
  //virtual void Initialize(void);
private:
  double Passband_Ripple_In_Db;
  double Stopband_Ripple_In_Db;
  double Selec_Factor;
  
};

#endif
