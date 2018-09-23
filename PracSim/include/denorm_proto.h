//
//  File = denorm_proto.h
//

#ifndef _DENORM_PROTO_H_
#define _DENORM_PROTO_H_

#include "filter_proto.h"
#include "filter_types.h"



class DenormalizedPrototype : public AnalogFilterPrototype
{
public:
  DenormalizedPrototype(  AnalogFilterPrototype *lowpass_proto_filt,
                          FILT_BAND_CONFIG_T filt_band_config,
                          double lower_passband_freq,
                          double upper_passband_freq );

  virtual ~DenormalizedPrototype();


protected:

  void LowpassDenormalize(void);
  void HighpassDenormalize(void);
  void BandpassDenormalize(void);
  void BandstopDenormalize(void);

  FILT_BAND_CONFIG_T Filt_Band_Config;
  double Passband_Edge;
  double Passband_Edge_2;
  double Warped_Passband_Edge;
  double Warped_Passband_Edge_2;
  AnalogFilterPrototype *Lowpass_Proto_Filt;
  
};

#endif
