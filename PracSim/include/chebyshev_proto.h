//
//  File = chebyshev_proto.h
//

#ifndef _CHEBYSHEV_PROTO_H_
#define _CHEBYSHEV_PROTO_H_

#include "filter_proto.h"


class ChebyshevPrototype : public AnalogFilterPrototype
{
public:
  ChebyshevPrototype( int prototype_order,
                      double ripple,
                      bool bw_is_ripple_bw );

  ~ChebyshevPrototype(void);
private:
  double Ripple;
  bool Bw_Is_Ripple_Bw;
  bool Bw_Is_3db_Bw;
  
};

#endif
