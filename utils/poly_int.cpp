// poly_int.cpp
//

#include <iostream>
using std::ofstream;
#include "poly_int.h"
#include <fstream>
extern ofstream DebugFile;

PolyOvrIntegers::PolyOvrIntegers(void)
{
  Degree=0;
  Coeff = new int[Degree+1];
  Coeff[0] = 0;
  
  Rem_Coeff = NULL;
}
//======================================
PolyOvrIntegers::PolyOvrIntegers( int degree)
{
  int i;
  Degree = degree;
  Coeff = new int[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff[i] = 0;
  Rem_Degree = 0;
  Rem_Coeff = new int[Rem_Degree+1];
  for(i=0; i<=Rem_Degree; i++)
      Rem_Coeff[i] = 0;
}
//======================================
PolyOvrIntegers::PolyOvrIntegers( int degree, 
                                  int coeff)
{
  int i;
  //cout << "in PolyOvrIntegers ctor(int,int)" << endl;
  Degree = degree;
  Coeff = new int[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff[i] = 0;
  Coeff[0] = -1;
  Coeff[degree] = coeff;
  Rem_Degree = 0;
  Rem_Coeff = new int[Rem_Degree+1];
}
int PolyOvrIntegers::MaxDegree(void)
{
  return Degree;
}

int PolyOvrIntegers::Coefficient(int degree)
{
  return Coeff[degree];
}
//========================================================
void PolyOvrIntegers::SetRemainder( PolyOvrIntegers* poly )
{
  //DebugFile << "Rem_Coeff = " << Rem_Coeff << endl;
  //cout << "Rem_Coeff = " << Rem_Coeff << endl;
  if(Rem_Coeff != NULL) delete[] Rem_Coeff;
  //DebugFile << "Rem_Coeff = " << Rem_Coeff << endl;
  //cout << "Rem_Coeff = " << Rem_Coeff << endl;
  Rem_Degree = poly->Degree;
  Rem_Coeff = new int[Rem_Degree+1];
  for(int i=0; i<=Rem_Degree; i++)
    {
    Rem_Coeff[i] = poly->Coeff[i];
    }
}
//========================================================
void PolyOvrIntegers::SetRemainder( int degree, 
                                    int* coeff )
{
  if(Rem_Coeff != NULL) delete[] Rem_Coeff;
  Rem_Degree = degree;
  Rem_Coeff = new int[Rem_Degree+1];
  for(int i=0; i<=Rem_Degree; i++)
    {
    Rem_Coeff[i] = coeff[i];
    }
}
//===========================================================
PolyOvrIntegers& PolyOvrIntegers::operator* 
                              (const PolyOvrIntegers &right)
{
  PolyOvrIntegers *result;

  result = new PolyOvrIntegers( Degree + right.Degree );

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
PolyOvrIntegers& PolyOvrIntegers::operator*= 
                            (const PolyOvrIntegers &right)
{
  //---------------------------------------------------
  // save pointer to original coefficient array so that
  // this array can be deleted once no longer needed

  int *orig_coeff = Coeff;
  int orig_degree = Degree;
  int i;

  //------------------------------------------------------
  // create new longer array to hold the new coefficients

  Degree += right.Degree;
  Coeff = new int[Degree+1];

  for( i=0; i<=Degree; i++)
      Coeff[i] = 0;

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
PolyOvrIntegers& PolyOvrIntegers::operator/ 
                            (const PolyOvrIntegers &divisor)
{
  int dividend_deg, divisor_deg, j, k;
  PolyOvrIntegers *result;

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;
  int *work = new int[dividend_deg+1];
  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree
    // of the dividend, the quotient is zero,
    // and the remainder is equal to the dividend

    PolyOvrIntegers *result = new PolyOvrIntegers(0);
    result->SetRemainder(this);
    }
  else
    {    
    // perform "synthetic" division to generate
    // quotient and remainder

    result = new PolyOvrIntegers( dividend_deg-divisor_deg);
    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff[j];
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      result->Coeff[k] = work[divisor_deg+k]/divisor.Coeff[divisor_deg];
      work[divisor_deg+k]=0;
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - (result->Coeff[k]) * divisor.Coeff[j-k];
      }
    result->Degree = dividend_deg-divisor_deg;
    int rem_deg = 0;
    for(j=0; j=dividend_deg ; j++)
      {
      if(work[j] != 0) rem_deg = j;
      }
    result->SetRemainder(rem_deg, work);
    }
  return *result;
}
//============================================================
PolyOvrIntegers& PolyOvrIntegers::operator/= 
                            (const PolyOvrIntegers &divisor)
{
  int dividend_deg, divisor_deg, j, k;

  //cout << "in PolyOvrIntegers operator/=" << endl;
  //------------------------------------------
  //

  if(Rem_Coeff != NULL) delete[] Rem_Coeff;
  dividend_deg = Degree;
  divisor_deg = divisor.Degree;
  int *work = new int[dividend_deg+1];

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree
    // of the dividend, the quotient is zero,
    // and the remainder is equal to the dividend

    Rem_Degree = Degree;
    Rem_Coeff = new int[Rem_Degree+1];
    }
  else
    {    
    // perform "synthetic" division to generate
    // quotient and remainder

    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff[j];
      Coeff[j] = 0;
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      Coeff[k] = work[divisor_deg+k]/divisor.Coeff[divisor_deg];
      work[divisor_deg+k]=0;
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - Coeff[k] * divisor.Coeff[j-k];
      }
    Degree = dividend_deg-divisor_deg;
    int rem_deg = 0;
    for(j=0; j<=dividend_deg ; j++)
      {
      if(work[j] != 0) rem_deg = j;
      }
    Rem_Degree = rem_deg;
    Rem_Coeff = new int[Rem_Degree+1];
    for(j=0; j<=rem_deg; j++)
      Rem_Coeff[j] = work[j];
    }
  return *this;
}
//=========================================================
//  dump polynomial to an output stream

void PolyOvrIntegers::DumpToStream( ostream* output_stream)
{
   (*output_stream) << "Degree = " << Degree << std::endl;
 
 for(int i=Degree; i>=0; i--)
   {
    (*output_stream) << "Coeff[" << i << "] = " 
       << Coeff[i] << std::endl;
   }
 return;
}
//===========================================================
PolyOvrIntegers* Derivative(const PolyOvrIntegers *right)
{
  PolyOvrIntegers *result;

  result = new PolyOvrIntegers( right->Degree-1 );

  for(int i=0; i<=right->Degree-1; i++)
    {
    (result->Coeff)[i] = (i+1)*((right->Coeff)[i+1]);
    }
  return result;
}
