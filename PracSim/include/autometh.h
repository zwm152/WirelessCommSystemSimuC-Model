//
//  File = autometh.h
//

#ifndef _AUTOMETH_H_
#define _AUTOMETH_H_

#include "toeplitz.h"
#include "typedefs.h"

class AutocorrMethCorrMtx : public ToeplitzMatrix
{
public:
  AutocorrMethCorrMtx( double *signal,
                       int seq_len,
                       int max_lag);
  ~AutocorrMethCorrMtx();

private:

};

#endif
