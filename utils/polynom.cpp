// polynom.cpp
//

//#include "stdafx.h"
#include "iostream.h"
#include "galfield.h"
#include "gfelem.h"
#include "polynom.h"

PolyOverGF::PolyOverGF(GaloisField* gf)
{
	pGalFld = gf;
  GaloisFieldElement* pLowDegElem = new GaloisFieldElement(gf);
  cout << "Hello from PolyOverGF ctor" << endl;
}

