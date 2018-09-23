//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = cmpxpoly.cpp
//
//

#include <math.h>
#include "cmpxpoly.h"
#include "laguerre.h"
#include "pause.h"
#include "stdlib.h"
#include "psstream.h"
#include <iostream>
using std::complex;
extern PracSimStream ErrorStream;
extern PracSimStream DetailedResults;

//======================================================
//  default constructor

CmplxPolynomial::CmplxPolynomial( )
{
   Poly_Degree = 0;
   Poly_Coeff = new complex<double>[1];
   Poly_Coeff[0] = complex<double>( 0.0, 0.0);
   RemCoeff = NULL;
   Root = NULL;
   return;
};
//======================================================
//  copy constructor

CmplxPolynomial::CmplxPolynomial( 
                     const CmplxPolynomial& original )
{
   int i;
   Poly_Degree = original.Poly_Degree;
   Poly_Coeff = new complex<double>[Poly_Degree+1];

   for( i=0; i<=Poly_Degree; i++)
      Poly_Coeff[i] = original.Poly_Coeff[i];

   if(original.Root != NULL){
      Root = new complex<double>[Poly_Degree];
      for( i=0; i<Poly_Degree; i++)
         Root[i] = original.Root[i];
   }
   return;
};
//======================================================
//  constructor for initializing a binomial

CmplxPolynomial
         ::CmplxPolynomial( const complex<double> coeff_1,
                            const complex<double> coeff_0 )
{
   Poly_Degree = 1;
   Poly_Coeff = new complex<double>[2];
   RemCoeff = NULL;
   Root = NULL;
   Poly_Coeff[0] = coeff_0;
   Poly_Coeff[1] = coeff_1;
   return;
}
//======================================================
//   initializing constructor - complex coeff

CmplxPolynomial
            ::CmplxPolynomial( const int degree,
                               const complex<double> *coeff)
{
   Poly_Degree = degree;
   Poly_Coeff = new complex<double>[degree+1];
   RemCoeff = NULL;
   Root = NULL;

   for(int i=0; i<=degree; i++) Poly_Coeff[i] = coeff[i]; 
   return;
}
//======================================================
//   initializing constructor - real coeff

CmplxPolynomial
            ::CmplxPolynomial( const double *coeff,
                               const int degree )
{
   Poly_Degree = degree;
   Poly_Coeff = new complex<double>[degree+1];
   RemCoeff = NULL;
   Root = NULL;

   for(int i=0; i<=degree; i++) 
            Poly_Coeff[i] = complex<double>(coeff[i],0.0);

   return;
};
//======================================================
//  assignment operator

CmplxPolynomial& CmplxPolynomial
            ::operator= (const CmplxPolynomial& right)
{
   if (Poly_Coeff != right.Poly_Coeff){
      //------------------------------------------------
      // Get rid of old coefficient array to make way 
      // for a new one of the correct length for the 
      // new polynomial being assigned 

      delete [] Poly_Coeff;
      delete [] Root;

      Poly_Degree = right.Poly_Degree;
      Poly_Coeff = new complex<double>[Poly_Degree+1];

      for( int i=0; i<=Poly_Degree; i++)
                        Poly_Coeff[i] = right.Poly_Coeff[i];
   }
   if(right.Root != NULL){
      Root = new complex<double>[Poly_Degree];
      int i;
      for( i=0; i<Poly_Degree; i++)
                        Root[i] = right.Root[i];
   }
   return *this;
}
//======================================================
// multiply assign operator        

CmplxPolynomial& CmplxPolynomial
         ::operator*= (const CmplxPolynomial &right)
{
   //---------------------------------------------------
   // save pointer to original coefficient array so that
   // this array can be deleted once no longer needed

   complex<double> *orig_coeff = Poly_Coeff;
   int orig_degree = Poly_Degree;

   //---------------------------------------------------
   // create new longer array to hold the new coeff 

   Poly_Degree += right.Poly_Degree;
   Poly_Coeff = new complex<double>[Poly_Degree+1];

   for( int i=0; i<=Poly_Degree; i++)
                  Poly_Coeff[i] = complex<double>(0.0, 0.0);

   //---------------------------------
   //  perform multiplication

   for(  int rgt_indx=0; 
            rgt_indx<= right.Poly_Degree; rgt_indx++){
      for( int orig_indx=0; 
               orig_indx <= orig_degree; orig_indx++){
         Poly_Coeff[orig_indx+rgt_indx] +=
            (orig_coeff[orig_indx] * 
            right.Poly_Coeff[rgt_indx]);
      }
   }
   return *this;
}    
//======================================================
// divide assign operator        

CmplxPolynomial& CmplxPolynomial
         ::operator/= (const CmplxPolynomial &divisor)
{
 //----------------------------------------------------
 //  In general, polynomial division will produce a
 //  quotient and a remainder.  This routine returns
 //  the quotient as its result.  The remainder will be
 //  stored in a member variable so that it can be
 //  checked or retrived by subsequent calls to the
 //  appropriate member functions.
 //-----------------------------------------------------
 // save pointer to original coefficient array so that 
 // this array can be deleted once no longer needed
 
 int dvdnd_deg, dvsr_deg, j, k;
 
 //-----------------------------------------------------
 //  create new array to hold the new coefficients 
 
 if(RemCoeff == NULL) RemCoeff = 
                        new std::complex<double>[Poly_Degree+1];
 dvdnd_deg = Poly_Degree;
 dvsr_deg = divisor.Poly_Degree;
 Poly_Degree -= dvsr_deg;
   
 //---------------------------------
 //  perform division
 
  for(j=0; j<=dvdnd_deg; j++){
    RemCoeff[j] = Poly_Coeff[j];
    Poly_Coeff[j] = complex<double>(0.0,0.0);
    }
  for( k=dvdnd_deg-dvsr_deg; k>=0; k--){
    Poly_Coeff[k] = RemCoeff[dvsr_deg+k]/
                     divisor.Poly_Coeff[dvsr_deg];
    for(j=dvsr_deg+k-1; j>=k; j--)
      RemCoeff[j] -= Poly_Coeff[k]*divisor.Poly_Coeff[j-k];
    }
 for(j=dvsr_deg; j<=dvdnd_deg; j++)
                RemCoeff[j] = complex<double>(0.0,0.0);
 return *this;
} 
//======================================================
//  Find roots of polynomial

void CmplxPolynomial::FindRoots( void )
{
   complex<double>* root;
   int status, i;
   CmplxPolynomial root_factor;
   root = new complex<double>[Poly_Degree];
   CmplxPolynomial work_poly;
   double epsilon=0.0000001;
   double epsilon2=1.0e-10;
   int max_iter=12;

   if(Root == NULL) Root = new complex<double>[Poly_Degree];
   //------------------------------------------------
   // find coarse locations for roots

   work_poly = CmplxPolynomial(Poly_Degree, Poly_Coeff);

   for(i=0; i<Poly_Degree-1; i++){
      Root[i] = complex<double>(0.0,0.0);
      status = LaguerreMethod( &work_poly, &(Root[i]),
                     epsilon, epsilon2, max_iter);
      if(status <0) {
         ErrorStream 
            << "Laguerre method did not converge"
            << endl;
         exit(75);
      }

      root_factor = 
         CmplxPolynomial( complex<double>(1.0,0.0),-Root[i]);
      work_poly /= root_factor;
      work_poly.DumpToStream(&cout);
      pause();
   }
   Root[Poly_Degree-1] = -(work_poly.GetCoeff(0));

   //------------------------------------------------
   //  polish the roots

   work_poly = CmplxPolynomial(Poly_Degree, Poly_Coeff);
   for(i=0; i<Poly_Degree; i++){
      status = LaguerreMethod( &work_poly, &(Root[i]), 
         epsilon, epsilon2, max_iter);
      if(status <0) {
         ErrorStream 
            << "Laguerre method did not converge" 
            << endl;
         exit(76);
      }
      DetailedResults << "Polished Root[" << i 
         << "] = (" 
         << Root[i].real() << ", " 
         << Root[i].imag() << ") " 
         << " (" << status << ")" << endl;
      pause();
   }
   return;
}
//======================================================
//  Get array of polynomial root values

complex<double> *CmplxPolynomial::GetRoots( void )
{
   complex<double>* root;
   int i;
   root = new complex<double>[Poly_Degree];
   if(Root == NULL) this->FindRoots();

   for(i=0; i<Poly_Degree; i++) root[i] = Root[i];
   return(root);
}
//======================================================
//  reflect root across the unit circle

void CmplxPolynomial::ReflectRoot( int root_idx )
{
   FindRoots();
   Root[root_idx] = complex<double>(1.0,0.0)/Root[root_idx];
   //BuildFromRoots();
}
//======================================================
//
int CmplxPolynomial::GetDegree(void)
{
   return(Poly_Degree);
}
//======================================================
//
complex<double> CmplxPolynomial::GetCoeff(int k)
{
   return Poly_Coeff[k];
}
//======================================================
//
void CmplxPolynomial::CopyCoeffs(complex<double> *coeff)
{
   for(int i=0; i<=Poly_Degree; i++) coeff[i] = Poly_Coeff[i];
   return;
}

//======================================================
//  dump polynomial to an output stream

void CmplxPolynomial::DumpToStream( ostream* output_stream)
{
 (*output_stream) << "Poly_Degree = " << Poly_Degree << endl;
 
 for(int i=Poly_Degree; i>=0; i--)
   {
    (*output_stream) << "Poly_Coeff[" << i << "] = " 
                     << Poly_Coeff[i].real() << ", "
                     << Poly_Coeff[i].imag() << endl;
   }
 return;
}  

//---------------------------------
//  force desired instantiations
