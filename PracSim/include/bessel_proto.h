//
// File = bessel_proto.h
//
#ifndef _BESSEL_PROTO_H_
#define _BESSEL_PROTO_H_  

#include "filter_proto.h"
#define MAX_BESSEL_ORDER 10

class BesselPrototype : public AnalogFilterPrototype
{
public: 
  // constructor to initialize for a specified filter order 
  BesselPrototype( int filt_order,
                   bool norm_for_delay );
  ~BesselPrototype();
}; 
#endif 