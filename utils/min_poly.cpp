//
// primpoly.cpp
//

#include <iostream>
#include <fstream>
#include "min_poly.h"
#include "ipow.h"
#include "poly_xf.h"
//using namespace std;
extern ::ofstream DebugFile;


//=====================================================
MinimalPolynomial::MinimalPolynomial( GaloisField* exten_field, 
                                      CyclotomicCoset *coset )
{
   int num_factors;
   int i,expon;
   PolyOvrExtenField** factors;
   ExtenFieldElem* element;
   num_factors = coset->size;
   factors = new PolyOvrExtenField*[num_factors];
  
   for(i=0; i<num_factors; i++){
      expon = coset->start[i];
      //element = new ExtenFieldElem(exten_field,expon);
      element = &(exten_field->GetElement(expon));
      factors[i] = new PolyOvrExtenField(exten_field, *element);
   }
   for(i=1; i<num_factors; i++){
      factors[0]->operator *=(*(factors[i]));
   }
   Coeff = new PrimeFieldElem[num_factors+1];
   for(i=0; i<=num_factors; i++){
      Coeff[i].Value = factors[0]->Coeff_Xf[i].Value[0].Value;
      Coeff[i].Modulus = factors[0]->Coeff_Xf[i].Value[0].Modulus;
   }
   Prime_Base = 2;
   Degree = num_factors;
   for(i=0; i<num_factors; i++){
      delete factors[i];
   }
   delete[] factors;

}

