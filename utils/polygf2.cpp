// polygf2.cpp
//

#include "iostream.h"
#include "polygf2.h"

PolyOvrGF2::PolyOvrGF2(void)
{
  
  /*
  Degree=7;
  Coeff = new int[Degree+1];
  Coeff[0] = 1;
  Coeff[1] = 1;
  Coeff[2] = 0;
  Coeff[3] = 0;
  Coeff[4] = 0;
  Coeff[5] = 0;
  Coeff[6] = 0;
  Coeff[7] = 1;
  */
  
  /*
  Degree=2;  // P(x)= x**2 + x + 2
  Coeff = new int[Degree+1];
  Coeff[0] = 2;
  Coeff[1] = 1;
  Coeff[2] = 1;
  */
  
  /*Degree=2;  // P(x)= x**2 + 2x + 2
  Coeff = new int[Degree+1];
  Coeff[0] = 2;
  Coeff[1] = 2;
  Coeff[2] = 1;
*/
  Degree=0;
  Coeff = new int[Degree+1];
  Coeff[0] = 0;
  
  /*
  Degree=3;
  Coeff = new int[Degree+1];
  Coeff[0] = 1;
  Coeff[1] = 0;
  Coeff[2] = 2;
  Coeff[3] = 1;
  */
  cout << "Hello from PolyOvrGF2 ctor" << endl;
  Rem_Coeff = NULL;
}
//======================================
PolyOvrGF2::PolyOvrGF2(int degree)
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
PolyOvrGF2::PolyOvrGF2(int degree, int coeff)
{
  int i;
  Degree = degree;
  Coeff = new int[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff[i] = 0;
  Coeff[degree] = coeff;
  Coeff[0] = 1;
  Rem_Degree = 0;
  Rem_Coeff = new int[Rem_Degree+1];
}
int PolyOvrGF2::MaxDegree(void)
{
  return Degree;
}

int PolyOvrGF2::Coefficient(int degree)
{
  return Coeff[degree];
}
//========================================================
void PolyOvrGF2::SetRemainder( PolyOvrGF2* poly )
{
  if(Rem_Coeff != NULL) delete[] Rem_Coeff;
  Rem_Degree = poly->Degree;
  Rem_Coeff = new int[Rem_Degree+1];
  for(int i=0; i<=Rem_Degree; i++)
    {
    Rem_Coeff[i] = poly->Coeff[i];
    }
}
//========================================================
void PolyOvrGF2::SetRemainder( int degree, int* coeff )
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
PolyOvrGF2& PolyOvrGF2::operator* (const PolyOvrGF2 &right)
{
  PolyOvrGF2 *result;

  result = new PolyOvrGF2( Degree + right.Degree );

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
PolyOvrGF2& PolyOvrGF2::operator*= (const PolyOvrGF2 &right)
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
PolyOvrGF2& PolyOvrGF2::operator/ (const PolyOvrGF2 &divisor)
{
  int dividend_deg, divisor_deg, j, k;
  PolyOvrGF2 *result;

  //------------------------------------------
  //  allocate new polynomial for result

 // PolyOvrGF2 *result = new PolyOvrGF2();

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;
  int *work = new int[dividend_deg+1];
  if(dividend_deg < divisor_deg)
    {
    // division does not take place.
    // remainder is equal to dividend
    PolyOvrGF2 *result = new PolyOvrGF2(0);
    result->SetRemainder(this);
    }
  else
    {    
    //--------------------------------
    // perform division

    result = new PolyOvrGF2(dividend_deg-divisor_deg);
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
PolyOvrGF2& PolyOvrGF2::operator/= (const PolyOvrGF2 &divisor)
{
  int dividend_deg, divisor_deg, j, k;

  //------------------------------------------
  //

  if(Rem_Coeff != NULL) delete[] Rem_Coeff;
  dividend_deg = Degree;
  divisor_deg = divisor.Degree;
  int *work = new int[dividend_deg+1];

  if(dividend_deg < divisor_deg)
    {
    // division does not take place.
    // remainder is equal to dividend
    Rem_Degree = Degree;
    Rem_Coeff = new int[Rem_Degree+1];
    }
  else
    {    
    //--------------------------------
    // perform division

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
    int rem_deg = 0;
    for(j=0; j<=dividend_deg ; j++)
      {
      if(work[j] != 0) rem_deg = j;
      }
    Rem_Degree = rem_deg;
    Rem_Coeff = new int[Rem_Degree+1];
    for(j=0; j=rem_deg; j++)
      Rem_Coeff[j] = work[j];
    }
  return *this;
}
//=========================================================
//  dump polynomial to an output stream

void PolyOvrGF2::DumpToStream( ostream* output_stream)
{
 (*output_stream) << "Degree = " << Degree << endl;
 
 for(int i=Degree; i>=0; i--)
   {
    (*output_stream) << "Coeff[" << i << "] = " 
                     << Coeff[i] << endl;
   }
 return;
}