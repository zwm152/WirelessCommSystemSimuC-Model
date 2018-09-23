//
//  File = Polynom.h
//

#ifndef _POLYNOM_H_
#define _POLYNOM_H_

#include "galfield.h"

class PolyOverGF
{
public:

  PolyOverGF( GaloisField *gf );

private:
  GaloisField* pGalFld;

};

#endif
