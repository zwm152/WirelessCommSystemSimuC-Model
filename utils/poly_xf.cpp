// poly_pf.cpp
//

#include <iostream>
#include <fstream>
#include "poly_xf.h"
#include "pfelem.h"
#include "stdlib.h"
extern ofstream DebugFile;

PolyOvrExtenField::PolyOvrExtenField( void )
{
  
  Exten_Field = NULL;
  Degree=0;
  Coeff_Xf = new ExtenFieldElem[Degree+1];
  Coeff_Xf[0] = ExtenFieldElem(Exten_Field,0); 
}
PolyOvrExtenField::PolyOvrExtenField(GaloisField *ext_field)
{
  
  Exten_Field = ext_field;
  Degree=0;
  Coeff_Xf = new ExtenFieldElem[Degree+1];
  Coeff_Xf[0] = ExtenFieldElem(Exten_Field,0); 
}
//======================================
PolyOvrExtenField::PolyOvrExtenField( GaloisField *ext_field,
                                      int degree)
{
  int i;
  Exten_Field = ext_field;
  Degree = degree;
  Coeff_Xf = new ExtenFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff_Xf[i] = ExtenFieldElem(Exten_Field,0);
}
//======================================
PolyOvrExtenField::PolyOvrExtenField( GaloisField *ext_field,
                                      int degree, 
                                      ExtenFieldElem coeff)
{
  int i;
  Exten_Field = ext_field;
  Degree = degree;
  Coeff_Xf = new ExtenFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
      Coeff_Xf[i] = ExtenFieldElem(Exten_Field,0);
  Coeff_Xf[degree] = coeff;
  Coeff_Xf[0] = ExtenFieldElem(Exten_Field,1);
}
//======================================
PolyOvrExtenField::PolyOvrExtenField( GaloisField *ext_field,
                                      ExtenFieldElem coeff)
{
  Exten_Field = ext_field;
  Degree = 1;
  Coeff_Xf = new ExtenFieldElem[Degree+1];
  Coeff_Xf[0] = coeff;
  Coeff_Xf[1] = ExtenFieldElem(Exten_Field,0);
}
#if 0
//============================================================
PolyOvrExtenField::PolyOvrExtenField( GaloisField *ext_field,
                                      PolyOvrIntegers *poly)
{
  int i, raw_val;
  Exten_Field = ext_field;
  Degree = poly->MaxDegree();
  Coeff_Xf = new ExtenFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
    {
    raw_val = (Prime_Base+(poly->Coefficient(i)))%Prime_Base;
    Coeff_Xf[i] = PrimeFieldElem(Prime_Base,raw_val);
    }
}
#endif
#if 0
//============================================================
PolyOvrExtenField::PolyOvrExtenField( int prime_base,
                                      int degree,
                                      rowvec_pf *coeff)
{
  int i;
  Prime_Base = prime_base;
  Degree = degree;
  Coeff_Xf = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
    {
    Coeff_Xf[i] = ((*coeff)[i]);
    }
}
//============================================================
PolyOvrExtenField::PolyOvrExtenField( int prime_base,
                                      rowvec_pf *coeff)
{
  int i;
  int len = coeff->getlen();
  Prime_Base = prime_base;
  Degree = 0;
  for(i=0; i<len; i++)
    {
    if( (*coeff)[i].Value != 0 ) Degree=i;
    }
  Coeff_Xf = new PrimeFieldElem[Degree+1];
  for(i=0; i<=Degree; i++)
    {
    Coeff_Xf[i] = (*coeff)[i];
    }
}
#endif
//============================================================
//int PolyOvrExtenField::PrimeBase(void)
//{
//  return Prime_Base;
//}

//============================================================
int PolyOvrExtenField::MaxDegree(void)
{
  return Degree;
}

//========================================================
int PolyOvrExtenField::NumberOfTerms(void)
{
  int num_terms=0;
  for(int i=0; i<=Degree; i++)
    {
    if( Coeff_Xf[i].Value != 0 ) num_terms++;
    }
  return(num_terms);
}
//========================================================
int PolyOvrExtenField::PenultimateDegree(void)
{
  int pen_deg=0;
  for(int i=Degree-1; i>=0; i--)
    {
    if( Coeff_Xf[i].Value == 0 ) continue;
      pen_deg = i;
      break;
    }
  return(pen_deg);
}
//========================================================
ExtenFieldElem PolyOvrExtenField::Coefficient(int degree)
{
  return Coeff_Xf[degree];
}
//========================================================
//void PolyOvrExtenField::GetCoeffs( ExtenFieldElem *coeff_vec)
//{
//  for(int i=0; i<=Degree; i++)
//    {
//    coeff_vec[i] = ExtenFieldElem(Exten_Field, Coeff_Xf[i].Value); 
//    }
//}

//===========================================================
PolyOvrExtenField& PolyOvrExtenField::operator* 
                              (const PolyOvrExtenField &right)
{
  PolyOvrExtenField *result;
  ExtenFieldElem work;

  result = new PolyOvrExtenField( Exten_Field, 
                                  Degree + right.Degree );

  //--------------------------------
  // perform multiplication

  for(int rgt_idx=0; rgt_idx<= right.Degree; rgt_idx++){
    for( int this_idx=0; this_idx <=Degree; this_idx++){
         work = (Coeff_Xf[this_idx] * right.Coeff_Xf[rgt_idx]);
         work = work + result->Coeff_Xf[this_idx+rgt_idx];
         result->Coeff_Xf[this_idx+rgt_idx] = work;
      //result->Coeff_Xf[this_idx+rgt_idx] +=
      //          (Coeff_Xf[this_idx] * right.Coeff_Xf[rgt_idx]);
      }
    }
  return *result;
}
//===========================================================
void PolyOvrExtenField::operator= 
                            (const PolyOvrExtenField &right)
{
  delete[] Coeff_Xf;

  Degree = right.Degree;
  Exten_Field = right.Exten_Field;
  Coeff_Xf = new ExtenFieldElem[Degree+1];

  for(int i=0; i<=Degree; i++)
      Coeff_Xf[i] = right.Coeff_Xf[i];

}
//===========================================================
PolyOvrExtenField& PolyOvrExtenField::operator*= 
                            (const PolyOvrExtenField &right)
{
   ExtenFieldElem work;
  //---------------------------------------------------
  // save pointer to original coefficient array so that
  // this array can be deleted once no longer needed

  ExtenFieldElem *orig_coeff = Coeff_Xf;
  int orig_degree = Degree;
  int i;

  //------------------------------------------------------
  // create new longer array to hold the new coefficients

  Degree += right.Degree;
  Coeff_Xf = new ExtenFieldElem[Degree+1];

  for( i=0; i<=Degree; i++)
      Coeff_Xf[i] = ExtenFieldElem(Exten_Field);
      //Coeff_Xf[i] = ExtenFieldElem(Exten_Field,0);

  //--------------------------------
  // perform multiplication

  for(int rgt_idx=0; rgt_idx<= right.Degree; rgt_idx++)
    {
    for( int orig_idx=0; orig_idx <=orig_degree; orig_idx++){
         work = Coeff_Xf[orig_idx+rgt_idx] + (orig_coeff[orig_idx] * right.Coeff_Xf[rgt_idx]);
      Coeff_Xf[orig_idx+rgt_idx] = work;
      //Coeff_Xf[orig_idx+rgt_idx] +=
      //          (orig_coeff[orig_idx] * right.Coeff_Xf[rgt_idx]);
      }
    }
  //delete []orig_coeff;
  return *this;
}
//===========================================================
PolyOvrExtenField& PolyOvrExtenField::operator-= 
                            (const ExtenFieldElem &const_term)
{

  Coeff_Xf[0] = Coeff_Xf[0] - const_term;
  return *this;
}
#if 0
//===========================================================
PolyOvrExtenField& PolyOvrExtenField::operator/ 
                            (const PolyOvrExtenField &divisor)
{
  int dividend_deg, divisor_deg, j, k;
  PolyOvrExtenField *result;

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree
    // of the dividend, the quotient is zero

    PolyOvrExtenField *result = new PolyOvrExtenField(Exten_Field,0);
    }
  else
    {    
    // perform "synthetic" division to generate quotient

    ExtenFieldElem *work = new ExtenFieldElem[dividend_deg+1];
    result = new PolyOvrExtenField( Exten_Field,
                                    dividend_deg-divisor_deg);
    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff_Xf[j];
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      result->Coeff_Xf[k] = work[divisor_deg+k]/divisor.Coeff_Xf[divisor_deg];
      work[divisor_deg+k]=ExtenFieldElem(Exten_Field,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - (result->Coeff_Xf[k]) * divisor.Coeff_Xf[j-k];
      }
    result->Degree = dividend_deg-divisor_deg;
    delete[] work;
    }
  return *result;
}
#endif
#if 0
//===========================================================
PolyOvrExtenField& PolyOvrExtenField::operator% 
                            (const PolyOvrExtenField &divisor)
{
  int dividend_deg, divisor_deg, j, k;
  PolyOvrExtenField *result;

  dividend_deg = Degree;
  divisor_deg = divisor.Degree;

  if(dividend_deg < divisor_deg)
    {
    // if degree of divisor is larger than degree of the
    // dividend, the remainder is equal to the dividend

    PolyOvrExtenField *result = new PolyOvrExtenField(Exten_Field,0);
    *result = *this;
    DebugFile << "Error - dividend smaller than divisor" << endl;
    exit(0);
    }
  else
    {    
    // perform "synthetic" division to genrate remainder

    ExtenFieldElem *work = new ExtenFieldElem[dividend_deg+1];
    ExtenFieldElem *quotient = new ExtenFieldElem[dividend_deg-divisor_deg+1];

    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff_Xf[j];
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      quotient[k] = work[divisor_deg+k]/divisor.Coeff_Xf[divisor_deg];
      work[divisor_deg+k]=ExtenFieldElem(Exten_Field,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - quotient[k] * divisor.Coeff_Xf[j-k];
      }
    //result->Degree = dividend_deg-divisor_deg;
    int rem_deg = 0;
    for(j=0; j<=dividend_deg ; j++)
      {
      if(!(work[j].EqualsZero())) rem_deg = j;
      }
    result = new PolyOvrExtenField( Exten_Field, rem_deg);
    for(j=0; j<=rem_deg; j++)
      {
      result->Coeff_Xf[j]=work[j];
      }
    delete[] work;
    delete[] quotient;
    }
  return *result;
}
#endif
#if 0
//============================================================
PolyOvrExtenField& PolyOvrExtenField::operator/= 
                            (const PolyOvrExtenField &divisor)
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
    Coeff_Xf[0] = ExtenFieldElem(Exten_Field, 0);
    }
  else
    {    
    // perform "synthetic" division to generate
    // quotient and remainder

    ExtenFieldElem *work = new ExtenFieldElem[dividend_deg+1];
    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff_Xf[j];
      Coeff_Xf[j] = ExtenFieldElem(Exten_Field,0);
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      Coeff_Xf[k] = work[divisor_deg+k]/divisor.Coeff_Xf[divisor_deg];
      work[divisor_deg+k]=ExtenFieldElem(Exten_Field,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - Coeff_Xf[k] * divisor.Coeff_Xf[j-k];
      }
    Degree = dividend_deg-divisor_deg;

    delete[] work;
    }
  return *this;
}
#endif
#if 0
//============================================================
PolyOvrExtenField& PolyOvrExtenField::operator%= 
                            (const PolyOvrExtenField &divisor)
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

    ExtenFieldElem *work = new ExtenFieldElem[dividend_deg+1];
    ExtenFieldElem *quotient = new ExtenFieldElem[dividend_deg-divisor_deg+1];

    for(j=0; j<= dividend_deg; j++)
      {
      work[j] = Coeff_Xf[j];
      //Coeff[j] = PrimeFieldElem(Prime_Base,0);
      }
    for(k=dividend_deg-divisor_deg; k>=0; k--)
      {
      quotient[k] = work[divisor_deg+k]/divisor.Coeff_Xf[divisor_deg];
      work[divisor_deg+k]=ExtenFieldElem(Exten_Field,0);
      for(j=divisor_deg+k-1; j>=k; j--)
        work[j] = work[j] - quotient[k] * divisor.Coeff_Xf[j-k];
      }
    //Degree = dividend_deg-divisor_deg;
    int rem_deg = 0;
    for(j=0; j<=dividend_deg ; j++)
      {
      if(!(work[j].EqualsZero())) rem_deg = j;
      }
    Degree = rem_deg;
    delete[] Coeff_Xf;
    Coeff_Xf = new ExtenFieldElem[Degree+1];
    for(j=0; j<=rem_deg; j++)
      Coeff_Xf[j] = work[j];

    delete[] work;
    delete[] quotient;
    }
  return *this;
}
#endif
//=========================================================
//  dump polynomial to an output stream

void PolyOvrExtenField::DumpToStream( ostream* output_stream)
{
 (*output_stream) << "Degree = " << Degree << endl;
 
 for(int i=Degree; i>=0; i--)
   {
    (*output_stream) << "Coeff[" << i << "] = " 
                     << Coeff_Xf[i] << endl;
   }
 return;
}
#if 0
//===========================================================
PolyOvrExtenField* Derivative(const PolyOvrExtenField *right)
{
  PolyOvrExtenField *result;

  result = new PolyOvrExtenField( right->Exten_Field, 
                                  right->Degree-1 );

  for(int i=0; i<=right->Degree-1; i++)
    {
    (result->Coeff_Xf)[i] = (i+1)*((right->Coeff_Xf)[i+1]);
    }
  return result;
}
#endif
#if 0
//===========================================================
PolyOvrExtenField& EuclideanAlgorithm( const PolyOvrExtenField *poly1,
                                       const PolyOvrExtenField *poly2)
{
  PolyOvrExtenField r_poly, u_poly, v_poly, *gcf_poly;

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
  gcf_poly = new PolyOvrExtenField(modulus,gcf_deg);
  for(int i=0; i<=gcf_deg; i++)
    {
    gcf_poly->Coeff[i] = v_poly.Coeff[i];
    }

  return( *gcf_poly );
}
#endif