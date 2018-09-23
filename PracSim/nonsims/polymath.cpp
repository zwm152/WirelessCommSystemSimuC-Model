// PolyMath.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include "galfield.h"
#include "pfelem.h"
#include "xfelem.h"
#include "poly_pf.h"
#include "primpoly.h"
#include "cycpoly.h"
#include "stdlib.h"
#include "berlefac.h"
#ifdef _DEBUG
 ::ofstream DebugFile("polymath.dbg", ios::out);
#endif

int main(int argc, char* argv[])
{
  int big_fld_deg;
  ExtenFieldElem xa,xb;
  ExtenFieldElem xc,xd;
  ExtenFieldElem xe,xf;
  cout << "Hello from main()" << endl;

  int pp_degree = 10;
  int prime_field_base = 2;

  PrimitivePolynomialSet* pp_set = 
                      new PrimitivePolynomialSet( prime_field_base, 
                                                  pp_degree);
  DebugFile << "simplest primitive poly:" << endl;
  (pp_set->GetSimplest())->DumpToStream(&DebugFile);

  big_fld_deg = pp_degree;
  GaloisField* big_field = new GaloisField( prime_field_base,
                                            big_fld_deg, 
                                            pp_set->GetSimplest());
  cout << "DONR " << endl;


  cin >> prime_field_base;
	return 0;
}

