// bchpoly.cpp
//

#include <iostream>
#include <fstream>
#include "cyclo_partit.h"
#include "galfield.h"
//#include "pfelem.h"
//#include "xfelem.h"
#include "poly_pf.h"
#include "min_poly.h"
//#include "cycpoly.h"
//#include "stdlib.h"
//#include "berlefac.h"
#ifdef _DEBUG
using namespace std;
 ofstream DebugFile("bchpoly.dbg", ios::out);
#endif

int main(int argc, char* argv[])
{
  int big_fld_deg;
  int big_field_expon;
  int num_cosets;
  int tau;
  bool *min_poly_needed;
  int coset_idx;
  int i,j;
  PolyOvrPrimeField *gener_poly;
  PolyOvrPrimeField *min_poly;
  PolyOvrPrimeField *prim_poly_to_define_gf;
  CyclotomicCoset *coset;
  GaloisField *exten_field;
  PrimeFieldElem** prim_poly_coeff;
//  ExtFieldElem xa,xb;
//  ExtFieldElem xc,xd;
//  ExtFieldElem xe,xf;
  cout << "Hello from main()" << endl;

 // int cyc_poly_number = 1023;
  int pp_degree = 10;
  int prime_field_base = 2;
  big_field_expon = 10;
  tau = 3;

  prim_poly_coeff = new PrimeFieldElem*[pp_degree+1];
  //for m=15, set coeffs 15, 1, 0
  //for m=10, set coeffs 10, 3, 0
  //for m=6, set coeffs 6, 1, 0
  //for m=4, set coeffs 4, 1, 0
  prim_poly_coeff[0] = new PrimeFieldElem(2,1);
  prim_poly_coeff[1] = new PrimeFieldElem(2,0);
  prim_poly_coeff[2] = new PrimeFieldElem(2,0);
  prim_poly_coeff[3] = new PrimeFieldElem(2,1);
  prim_poly_coeff[4] = new PrimeFieldElem(2,0);
  prim_poly_coeff[5] = new PrimeFieldElem(2,0);
  prim_poly_coeff[6] = new PrimeFieldElem(2,0);
  prim_poly_coeff[7] = new PrimeFieldElem(2,0);
  prim_poly_coeff[8] = new PrimeFieldElem(2,0);
  prim_poly_coeff[9] = new PrimeFieldElem(2,0);
  prim_poly_coeff[10] = new PrimeFieldElem(2,1);

  prim_poly_to_define_gf = new PolyOvrPrimeField(prime_field_base, pp_degree, prim_poly_coeff);

  exten_field = new GaloisField( prime_field_base,big_field_expon,prim_poly_to_define_gf);

  CyclotomicPartition* coset_partition = 
                      new CyclotomicPartition( prime_field_base, 
                                               big_field_expon);
  num_cosets = coset_partition->GetNumCosets();

  min_poly_needed = new bool[num_cosets];
  for( i=0; i<num_cosets; i++){
     min_poly_needed[i] = false;
  }

  for( j=0; j<=tau; j++){
     coset_idx = coset_partition->GetCosetIndex(j);
     min_poly_needed[coset_idx] = true;
  }

  gener_poly = NULL;
  for( j=1; j<=tau; j++){
     if(min_poly_needed[j]){
        coset = coset_partition->GetSpecificCoset(j);
        if(gener_poly == NULL){
           gener_poly = new MinimalPolynomial(exten_field, coset);
        }
        else{
           min_poly = new MinimalPolynomial(exten_field, coset);
           (*gener_poly) *= (*min_poly);
           delete min_poly;
        }
     }
  }
  cin >> prime_field_base;
	return 0;
}

