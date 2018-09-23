//
//  File = elip_filt_intg.h
//

#ifndef _ELIP_FILT_INTG_H_
#define _ELIP_FILT_INTG_H_

#include "anlg_filt_intg.h"
#include "psmodel.h"

class EllipticalFilterByInteg : public AnalogFilterByInteg
{
public:
  EllipticalFilterByInteg( char* instance_name,
                   PracSimModel *outer_model,
                   Signal<float> *in_sig,
                   Signal<float> *out_sig );

//  EllipticalFilter( char *instance_name,
//                    int kind_of_interface,
//                    int filter_band_config,
//                    int filter_order,
//                    float upper_passband_edge,
//                    float lower_passband_edge,
//                    bool bypass_enabled);
  ~EllipticalFilterByInteg(void);
  //virtual void Initialize(void);
private:
  double Passband_Ripple_In_Db;
  double Stopband_Ripple_In_Db;
  double Selec_Factor;
  
};

#endif
