//
//  File = butterworth_proto.h
//

#ifndef _BUTTERWORTH_PROTO_H_
#define _BUTTERWORTH_PROTO_H_

#include "filter_proto.h"


class ButterworthPrototype : public AnalogFilterPrototype
{
public:
  ButterworthPrototype( int prototype_order );

  ~ButterworthPrototype(void);
  
};

#endif
