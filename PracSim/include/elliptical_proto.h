//
//  File = elliptical_proto.h
//

#ifndef _ELLIPTICAL_PROTO_H_
#define _ELLIPTICAL_PROTO_H_

#include "filter_proto.h"


double ipow(double x, int m);
class EllipticalPrototype : public AnalogFilterPrototype
{
public:
  EllipticalPrototype(  int filt_order,
                        double passband_ripple,
                        double stopband_ripple,
                        double selec_factor );

  ~EllipticalPrototype(void);
private:
  //double Ripple;
  
};

#endif
