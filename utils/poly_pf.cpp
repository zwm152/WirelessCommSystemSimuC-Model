// poly_pf.cpp
//

#include <iostream>
#include <fstream>
#include "poly_pf.h"
#include "pfelem.h"
#include "stdlib.h"
extern ofstream DebugFile;

PolyOvrPrimeField::PolyOvrPrimeField( void )
{
  
  Prime_Base = 9999;
  Degree=0;
  Coeff = new PrimeFieldElem[Degree+1];
  Coeff[0] = PrimeFieldElem(Prime_Base,0); 
}
PolyOvrPrimeField::PolyOvrPrimeField(int prime_base)
{
  
  Prime_Base = prime_base;
  Degree=0;
  Coeff = new PrimeFieldElem[Degree+1];
  Coeff[0] = PrimeFieldElem(Prime_Base,0); 
}
//======================================
PolyOvrPrimeField::PolyOvrPrimeField( int prime_base,
                                      int degree)
{
  int i;
  Prime_Base = prime_base;
  Degree = degree;
  Coeff = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff[i] = PrimeFieldElem(Prime_Base,0);
}
//======================================
PolyOvrPrimeField::PolyOvrPrimeField( int prime_base,
                                      int degree, 
                                      PrimeFieldElem coeff)
{
  int i;
  Prime_Base = prime_base;
  Degree = degree;
  Coeff = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff[i] = PrimeFieldElem(Prime_Base,0);
  Coeff[degree] = coeff;
  Coeff[0] = PrimeFieldElem(Prime_Base,1);
}
//======================================
PolyOvrPrimeField::PolyOvrPrimeField( int prime_base,
                                      int degree, 
                                      PrimeFieldElem* coeff[])
{
  int i;
  Prime_Base = prime_base;
  Degree = degree;
  Coeff = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff[i] = *(coeff[i]);
}
//============================================================
PolyOvrPrimeField::PolyOvrPrimeField( int prime_base,
                                      PolyOvrIntegers *poly)
{
  int i, raw_val;
  Prime_Base = prime_base;
  Degree = poly->MaxDegree();
  Coeff = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
    {
    raw_val = (Prime_Base+(poly->Coefficient(i)))%Prime_Base;
    Coeff[i] = PrimeFieldElem(Prime_Base,raw_val);
    }
}
//============================================================
int PolyOvrPrimeField::PrimeBase(void)
{
  return Prime_Base;
}

//============================================================
int PolyOvrPrimeField::MaxDegree(void)
{
  return Degree;
}

//========================================================
int PolyOvrPrimeField::NumberOfTerms(void)
{
  int num_terms=0;
  for(int i=0; i<=Degree; i++)
    {
    if( Coeff[i].Value != 0 ) num_terms++;
    }
  return(num_terms);
}
//========================================================
int PolyOvrPrimeField::PenultimateDegree(void)
{
  int pen_deg=0;
  for(int i=Degree-1; i>=0; i--)
    {
    if( Coeff[i].Value == 0 ) continue;
      pen_deg = i;
      break;
    }
  return(pen_deg);
}
//========================================================
PrimeFieldElem PolyOvrPrimeField::Coefficient(int degree)
{
  return Coeff[degree];
}
//========================================================
void PolyOvrPrimeField::GetCoeffs( PrimeFieldElem *coeff_vec)
{
  for(int i=0; i<=Degree; i++)
    {
    coeff_vec[i] = PrimeFieldElem(Prime_Base, Coeff[i].Value); 
    }
}

//===========================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator* 
                              (const PolyOvrPrimeField &right)
{
  PolyOvrPrimeField *result;

  result = new PolyOvrPrimeField( Prime_Base, 
                                  Degree + right.Degree );

  //--------------------------------
  // perform multiplication

  for(int rgt_idx=0; rgt_idx<= right.Degree; rgt_idx++)
    {
    for( int this_idx=0; this_idx <=Degree; this_idx++)
      {
      result->Coeff[this_idx+rgt_idx] +=
                (Coeff[this_idx] * right.Coeff[rgt_idx]);
      }
    }
  return *result;
}
//===========================================================
void PolyOvrPrimeField::operator= 
                            (const PolyOvrPrimeField &right)
{
  delete[] Coeff;

  Degree = right.Degree;
  Prime_Base = right.Prime_Base;
  Coeff = new PrimeFieldElem[Degree+1];

  for(int i=0; i<=Degree; i++)
      Coeff[i] = right.Coeff[i];

}
//===========================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator*= 
                            (const PolyOvrPrimeField &right)
{
  //---------------------------------------------------
  // save pointer to original coefficient array so that
  // this array can be deleted once no longer needed

  PrimeFieldElem *orig_coeff = Coeff;
  int orig_degree = Degree;
  int i;

  //------------------------------------------------------
  // create new longer array to hold the new coefficients

  Degree += right.Degree;
  Coeff = new PrimeFieldElem[Degree+1];

  for( i=0; i<=Degree; i++)
      Coeff[i] = PrimeFieldElem(Prime_Base,0);

  //--------------------------------
  // perform multiplication

  for(int rgt_idx=0; rgt_idx<= right.Degree; rgt_idx++)
    {
    for( int orig_idx=0; orig_idx <=orig_degree; orig_idx++)
      {
      Coeff[orig_idx+rgt_idx] +=
                (orig_coeff[orig_idx] * right.Coeff[rgt_idx]);
      }
    }
  //delete []orig_coeff;
  return *this;
}
//===========================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator-= 
                            (const PrimeFieldElem &const_term)
{

  Coeff[0] -= const_term;
  return *this;
}
//===========================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator/ 
                            (const PolyOvrPrimeField &divisor)
{
  int dividend_deg, divisor_deg, j, k;
  PolyOvrPrimeField *result;

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree
    // of the dividend, the quotient is zero

    PolyOvrPrimeField *result = new PolyOvrPrimeField(Prime_Base,0);
    }
  else
    {    
    // perform "synthetic" division to generate quotient

    PrimeFieldElem *work = new PrimeFieldElem[dividend_deg+1];
    result = new PolyOvrPrimeField( Prime_Base,
                                    dividend_deg-divisor_deg);
    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff[j];
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      result->Coeff[k] = work[divisor_deg+k]/divisor.Coeff[divisor_deg];
      work[divisor_deg+k]=PrimeFieldElem(Prime_Base,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - (result->Coeff[k]) * divisor.Coeff[j-k];
      }
    result->Degree = dividend_deg-divisor_deg;
    delete[] work;
    }
  return *result;
}
//===========================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator% 
                            (const PolyOvrPrimeField &divisor)
{
  int dividend_deg, divisor_deg, j, k;
  PolyOvrPrimeField *result;

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree of the
    // dividend, the remainder is equal to the dividend

    PolyOvrPrimeField *result = new PolyOvrPrimeField(Prime_Base,0);
    *result = *this;
    DebugFile << "Error - dividend smaller than divisor" << endl;
    exit(0);
    }
  else
    {    
    // perform "synthetic" division to genrate remainder

    PrimeFieldElem *work = new PrimeFieldElem[dividend_deg+1];
    PrimeFieldElem *quotient = new PrimeFieldElem[dividend_deg-divisor_deg+1];

    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff[j];
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      quotient[k] = work[divisor_deg+k]/divisor.Coeff[divisor_deg];
      work[divisor_deg+k]=PrimeFieldElem(Prime_Base,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - quotient[k] * divisor.Coeff[j-k];
      }
    //result->Degree = dividend_deg-divisor_deg;
    int rem_deg = 0;
    for(j=0; j<=dividend_deg ; j++)
      {
      if(work[j] != 0) rem_deg = j;
      }
    result = new PolyOvrPrimeField( Prime_Base, rem_deg);
    for(j=0; j<=rem_deg; j++)
      {
      result->Coeff[j]=work[j];
      }
    delete[] work;
    delete[] quotient;
    }
  return *result;
}
//============================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator/= 
                            (const PolyOvrPrimeField &divisor)
{
  int dividend_deg, divisor_deg, j, k;

  //------------------------------------------
  //

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree
    // of the dividend, the quotient is zero,
    // and the remainder is equal to the dividend
    Degree = 0;
    Coeff[0] = PrimeFieldElem(Prime_Base, 0);
    }
  else
    {    
    // perform "synthetic" division to generate
    // quotient and remainder

    PrimeFieldElem *work = new PrimeFieldElem[dividend_deg+1];
    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff[j];
      Coeff[j] = PrimeFieldElem(Prime_Base,0);
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      Coeff[k] = work[divisor_deg+k]/divisor.Coeff[divisor_deg];
      work[divisor_deg+k]=PrimeFieldElem(Prime_Base,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - Coeff[k] * divisor.Coeff[j-k];
      }
    Degree = dividend_deg-divisor_deg;

    delete[] work;
    }
  return *this;
}
//============================================================
PolyOvrPrimeField& PolyOvrPrimeField::operator%= 
                            (const PolyOvrPrimeField &divisor)
{
  int dividend_deg, divisor_deg, j, k;

  //------------------------------------------
  //

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree of the
    // dividend, the remainder is equal to the dividend
    }
  else
    {    
    // perform "synthetic" division to generate
    // quotient and remainder

    PrimeFieldElem *work = new PrimeFieldElem[dividend_deg+1];
    PrimeFieldElem *quotient = new PrimeFieldElem[dividend_deg-divisor_deg+1];

    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff[j];
      //Coeff[j] = PrimeFieldElem(Prime_Base,0);
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      quotient[k] = work[divisor_deg+k]/divisor.Coeff[divisor_deg];
      work[divisor_deg+k]=PrimeFieldElem(Prime_Base,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - quotient[k] * divisor.Coeff[j-k];
      }
    //Degree = dividend_deg-divisor_deg;
    int rem_deg = 0;
    for(j=0; j<=dividend_deg ; j++)
      {
      if(work[j] != 0) rem_deg = j;
      }
    Degree = rem_deg;
    delete[] Coeff;
    Coeff = new PrimeFieldElem[Degree+1];
    for(j=0; j<=rem_deg; j++)
      Coeff[j] = work[j];

    delete[] work;
    delete[] quotient;
    }
  return *this;
}
//=========================================================
//  dump polynomial to an output stream

void PolyOvrPrimeField::DumpToStream( ostream* output_stream)
{
 (*output_stream) << "Degree = " << Degree << endl;
 
 for(int i=Degree; i>=0; i--)
   {
    (*output_stream) << "Coeff[" << i << "] = " 
                     << Coeff[i] << endl;
   }
 return;
}
//===========================================================
PolyOvrPrimeField* Derivative(const PolyOvrPrimeField *right)
{
  PolyOvrPrimeField *result;

  result = new PolyOvrPrimeField( right->Prime_Base, 
                                  right->Degree-1 );

  for(int i=0; i<=right->Degree-1; i++)
    {
    (result->Coeff)[i] = (i+1)*((right->Coeff)[i+1]);
    }
  return result;
}
//===========================================================
PolyOvrPrimeField& EuclideanAlgorithm( const PolyOvrPrimeField *poly1,
                                       const PolyOvrPrimeField *poly2)
{
  PolyOvrPrimeField r_poly, u_poly, v_poly, *gcf_poly;

  if( (poly1->Degree) > (poly2->Degree) )
    {
    u_poly = *poly1;
    v_poly = *poly2;
    }
  else
    {
    u_poly = *poly2;
    v_poly = *poly1;
    }
  int modulus = u_poly.Prime_Base;

  bool done = false;

  while( !done )
    {
    // calculate remainder of u/v

    r_poly = u_poly % v_poly;

    //DebugFile << "u_poly: " << endl;
    //u_poly.DumpToStream(&DebugFile);
    //DebugFile << "v_poly: " << endl;
    //v_poly.DumpToStream(&DebugFile);
    //DebugFile << "r_poly: " << endl;
    //r_poly.DumpToStream(&DebugFile);

    // examine remainder

    if( r_poly.Degree == 0 )
      {
      done = true;
      if( r_poly.Coeff[0].Value != 0 )
        {
        // set gcf to 1
        v_poly.Degree = 0;
        v_poly.Coeff[0].Value = 1;
//        (v_poly->Coeff[0]).Modulus = modulus;
// this should be okay as is, and it will be OBE when deep-copy
// assignments are implemented
        }
      }
    else
      {
      u_poly = v_poly;
      v_poly = r_poly;
      }
    } // end of while( !done )
  int gcf_deg = v_poly.Degree;
  gcf_poly = new PolyOvrPrimeField(modulus,gcf_deg);
  for(int i=0; i<=gcf_deg; i++)
    {
    gcf_poly->Coeff[i] = v_poly.Coeff[i];
    }

  return( *gcf_poly );
}