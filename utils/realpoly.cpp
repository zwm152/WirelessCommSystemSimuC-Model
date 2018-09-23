//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = poly.cpp
//
//  class that implements a polynomial with
//  real-valued coefficients
//

#include <math.h>
#include "realpoly.h"

//======================================================
//  default constructor

RealPolynomial::RealPolynomial( )
{
 Poly_Degree = 0;
 Poly_Coeff = new double[1];
 Poly_Coeff[0] = 0.0;
 return;
};
//===================================================================
//  initializing constructor
RealPolynomial::RealPolynomial( const int degree, const double* const coefficient)
  {
  Poly_Degree  = degree;
  Poly_Coeff = new double[Poly_Degree+1];

  for(int idx=0; idx<=degree; idx++){
    Poly_Coeff[idx] = coefficient[idx];
    }
  };
//===================================================================
//  constructor for initializing a constant

RealPolynomial::RealPolynomial( const double coeff_0 )
{
 Poly_Degree = 0;
 Poly_Coeff = new double[1];
 Poly_Coeff[0] = coeff_0;
 return;
};
//===================================================================
//  copy constructor

RealPolynomial::RealPolynomial( const RealPolynomial& original )
{
 Poly_Degree = original.Poly_Degree;
 Poly_Coeff = new double[Poly_Degree+1];
 
 for( int i=0; i<=Poly_Degree; i++){
    Poly_Coeff[i] = original.Poly_Coeff[i];
   }
 return;
};

//===================================================================
//  conversion constructors

RealPolynomial::RealPolynomial( const CmplxPolynomial& original )
{
 Poly_Degree = original.Poly_Degree;
 Poly_Coeff = new double[Poly_Degree+1];
 
 for( int i=0; i<=Poly_Degree; i++){
    Poly_Coeff[i] = (original.Poly_Coeff[i]).real();
   }
 return;
};
//===================================================================
//  constructor for initializing a binomial

RealPolynomial::RealPolynomial( const double coeff_1,
                        const double coeff_0 )
{
 Poly_Degree = 1;
 Poly_Coeff = new double[2];
 
 Poly_Coeff[0] = coeff_0;
 Poly_Coeff[1] = coeff_1;
 
 return;
}
//===================================================================
//  constructor for initializing a quadratic

RealPolynomial::RealPolynomial( const double coeff_2,
                        const double coeff_1,
                        const double coeff_0 )
{
 Poly_Degree = 2;
 Poly_Coeff = new double[3];
 
 Poly_Coeff[0] = coeff_0;
 Poly_Coeff[1] = coeff_1;
 Poly_Coeff[2] = coeff_2;
 
 return;
}
//=================================================================
//  assignment operator

RealPolynomial& RealPolynomial::operator= (const RealPolynomial& right)
{
 if (Poly_Coeff != right.Poly_Coeff)
   {
    //-------------------------------------------------------------
    // Get rid of old coefficient array to make way for a new one
    // of the correct length for the new polynomial being assigned 
    
    delete [] Poly_Coeff;
    
    Poly_Degree = right.Poly_Degree;
    Poly_Coeff = new double[Poly_Degree+1];
    
    for( int i=0; i<=Poly_Degree; i++){
       Poly_Coeff[i] = right.Poly_Coeff[i];
      }
   }
 return *this;
}

//===================================================================
// multiply assign operator        

RealPolynomial& RealPolynomial::operator*= (const RealPolynomial &right)
{
 //-----------------------------------------------------
 // save pointer to original coefficient array so that 
 // this array can be deleted once no longer needed
 
 double *orig_coeff = Poly_Coeff;
 int orig_degree = Poly_Degree;
 
 //-------------------------------------------------------
 //  create new longer array to hold the new coefficients 
 
 Poly_Degree += right.Poly_Degree;
 Poly_Coeff = new double[Poly_Degree+1];
 
 for( int i=0; i<=Poly_Degree; i++)
    Poly_Coeff[i] = 0.0;
    
 //---------------------------------
 //  perform multiplication
 
 for( int rgt_indx=0; rgt_indx<= right.Poly_Degree; rgt_indx++){
    for( int orig_indx=0; orig_indx <= orig_degree; orig_indx++){
       Poly_Coeff[orig_indx+rgt_indx] +=
              (orig_coeff[orig_indx] * right.Poly_Coeff[rgt_indx]);
      }
   }

 return *this;
}    
//===================================================================
// add assign operator        

RealPolynomial& RealPolynomial::operator+= (const RealPolynomial &right)
{
  //-----------------------------------------------------
  // save pointer to original coefficient array so that 
  // this array can be deleted once no longer needed

  double *orig_coeff = Poly_Coeff;
  int orig_degree = Poly_Degree;
 
  if(right.Poly_Degree > Poly_Degree){
    Poly_Degree = right.Poly_Degree;
    Poly_Coeff = new double[Poly_Degree+1];

    for( int i=0; i<=orig_degree; i++)
      Poly_Coeff[i] = orig_coeff[i] + right.Poly_Coeff[i];
    for( i=orig_degree+1; i<=Poly_Degree; i++)
      Poly_Coeff[i] = right.Poly_Coeff[i];
    delete[] orig_coeff;
    }
  else{
    for(int i=0; i<=right.Poly_Degree; i++)
      Poly_Coeff[i] += right.Poly_Coeff[i];
    }
 return *this;
}    
//===================================================================
// multiply by another polynomial

RealPolynomial RealPolynomial::operator *(const RealPolynomial &right)
{
  RealPolynomial result(Poly_Degree, Poly_Coeff);
  result *= right;
  return result;
}
//===================================================================
// add another polynomial

RealPolynomial RealPolynomial::operator +(const RealPolynomial &right)
{
  RealPolynomial result(Poly_Degree, Poly_Coeff);
  result += right;
  return result;
}
//===================================================================
// multiply by double and assign operator        

RealPolynomial& RealPolynomial::operator*= (double right)
{
 //---------------------------------
 //  perform multiplication
 
 for( int indx=0; indx<= Poly_Degree; indx++){
   Poly_Coeff[indx] *= right;
   }

 return *this;
}    
//===================================================================
// multiply RealPolynomial times double        

RealPolynomial operator* (const RealPolynomial &poly, const double d_val)
{
  RealPolynomial result(poly);
  result *= d_val;
  return result;
}    
//===================================================================
// multiply double times RealPolynomial        

RealPolynomial operator* (const double d_val, const RealPolynomial &poly)
{
  RealPolynomial result(poly);
  result *= d_val;
  return result;
}    
//===================================================================
// divide assign operator        

RealPolynomial& RealPolynomial::operator/= (const RealPolynomial &right)
{
 //-----------------------------------------------------
 // save pointer to original coefficient array so that 
 // this array can be deleted once no longer needed
 
 double *orig_coeff = Poly_Coeff;
 int orig_degree = Poly_Degree;
 
 //-------------------------------------------------------
 //  create new longer array to hold the new coefficients 
 
 Poly_Degree += right.Poly_Degree;
 Poly_Coeff = new double[Poly_Degree+1];
 
 for( int i=0; i<=Poly_Degree; i++)
    Poly_Coeff[i] = 0.0;
    
 //---------------------------------
 //  perform multiplication
 
 for( int rgt_indx=0; rgt_indx<= right.Poly_Degree; rgt_indx++){
    for( int orig_indx=0; orig_indx <= orig_degree; orig_indx++){
       Poly_Coeff[orig_indx+rgt_indx] +=
              (orig_coeff[orig_indx] * right.Poly_Coeff[rgt_indx]);
      }
   }

 return *this;
}    
//=========================================================
//  dump polynomial to an output stream

void RealPolynomial::DumpToStream( ofstream* output_stream)
{
 (*output_stream) << "Poly_Degree = " << Poly_Degree << endl;
 
 for(int i=Poly_Degree; i>=0; i--){
    (*output_stream) << "Coeff[" << i << "] = " 
                     << Poly_Coeff[i] << endl;
   }
 return;
}  
//====================================================
//

int RealPolynomial::GetDegree(void)
{
   return(Poly_Degree);
}

//==================================================
//

double RealPolynomial::GetCoefficient(int k)
{
   return Poly_Coeff[k];
}

