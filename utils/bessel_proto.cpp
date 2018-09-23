//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = bessel_proto.cpp
//
//  Prototype Bessel Filter
//

#include <math.h>
#include "parmfile.h"
#include <stdlib.h>
#include "bessel_proto.h"
#include "ipow.h"
#include "filter_types.h"
//#include "elipfunc.h"
#include "laguerre.h"
//#include "complex.h"
//#include "cmpxpoly.h"

extern ParmFile ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

//======================================================
//  constructor

BesselPrototype::BesselPrototype( int filt_order, 
                                  bool norm_for_delay)
                :AnalogFilterPrototype(filt_order)
{
  int indx, indx_m1, indx_m2;
  int i, n, ii, work_order;
  double epsilon, epsilon2;
  int max_iter, laguerre_status;
  long q_poly[3][MAX_BESSEL_ORDER];
  std::complex<double> *denom_poly, *work_coeff;
  std::complex<double> root, work1, work2;
  double renorm_val, smallest;
  CmplxPolynomial *work_poly;

  //-------------------------------------------------------
  //  these values are reciprocals of values in Table 5.10

  double renorm_factor[9] = { 0.0,     0.0,     0.72675, 
                              0.57145, 0.46946, 0.41322, 
                              0.37038, 0.33898, 0.31546};

 Pole_Locs = new std::complex<double>[Filt_Order+1];
 Pole_Locs[0] = Filt_Order;
 Num_Poles = Filt_Order;
 Zero_Locs = new std::complex<double>[1];
 Zero_Locs[0] = 0;
 Num_Zeros = 0;
 
 H_Zero = 1.0;
  denom_poly = new std::complex<double>[MAX_BESSEL_ORDER];
  //Denom_Poly = new double[Filt_Order+1];
  work_coeff = new std::complex<double>[MAX_BESSEL_ORDER];

  indx = 1;
  indx_m1 = 0;
  indx_m2 = 2;
  renorm_val = renorm_factor[Filt_Order];

  //-----------------------------------------
  // initialize polynomials for n=0 and n=1

  for( i=0; i<(3*MAX_BESSEL_ORDER) ; i++) 
      q_poly[0][i] = 0;

  q_poly[0][0] = 1;
  q_poly[1][0] = 1;
  q_poly[1][1] = 1;
  RealPolynomial qq_poly[3];
  qq_poly[0] = RealPolynomial(1.0);
  *DebugFile << "qq_poly[0]" << endl;
  qq_poly[0].DumpToStream(DebugFile);
  qq_poly[1] = RealPolynomial(1.0, 1.0);
  *DebugFile << "qq_poly[1]" << endl;
  qq_poly[1].DumpToStream(DebugFile);

  //----------------------------------------------
  //  compute polynomial using recursion from n=2
  //  up through n=order

  RealPolynomial s_sqrd(1.0, 0.0, 0.0);
  for( n=2; n<=filt_order; n++)
    {
    indx = (indx+1)%3;
    indx_m1 = (indx_m1+1)%3;
    indx_m2 = (indx_m2+1)%3;

    qq_poly[indx] =  double(2*n-1) * qq_poly[indx_m1];

    for( i=0; i<n; i++)
      {
      q_poly[indx][i] = (2*n-1) * 
                             q_poly[indx_m1][i];
      }
    qq_poly[indx] += qq_poly[indx_m2] * s_sqrd;

    for( i=2; i<=n; i++)
      {
      q_poly[indx][i] = q_poly[indx][i] + 
                             q_poly[indx_m2][i-2];
      }
    }

  #ifdef _DEBUG
    for( i=0; i<=Filt_Order; i++)
      {
      *DebugFile << "q_poly[" << i << "] = "
                << q_poly[indx][i] << endl;
      }
    qq_poly[indx].DumpToStream(DebugFile);
  #endif


  double* denom_coef = new double[filt_order+1];
  if(norm_for_delay)
    {
    for( i=0; i<=filt_order; i++)
      {
      denom_coef[i] = qq_poly[indx].GetCoefficient(i);
      denom_poly[i] = std::complex<double>(denom_coef[i], 0.0);
      //denom_poly[i] = std::complex<double>(double(q_poly[indx][i]), 0.0);
      #ifdef _DEBUG
      *DebugFile << "q_poly[" << i << "] = "
                << q_poly[indx][i] << endl;
      #endif
      }
    }
  else
    {
    for( i=0; i<=filt_order; i++)
      {
      denom_coef[i] = qq_poly[indx].GetCoefficient(i) 
                      * ipow(renorm_val, (filt_order-i));
      denom_poly[i] = std::complex<double>(denom_coef[i], 0.0);
      }
    }
  Denom_Poly = RealPolynomial(filt_order, denom_coef);
  Degree_Of_Denom = filt_order;
  //delete [] denom_coef;
  #ifdef _DEBUG
    for( i=0; i<=filt_order; i++)
      *DebugFile << "denom_coef[" << i << "] = "
                << denom_coef[i] << endl;
    for( i=0; i<=filt_order; i++)
      *DebugFile << "denom_poly[" << i << "] = "
                << (denom_poly[i].real()) << endl;
  #endif
  H_Zero = denom_poly[0].real();
  //---------------------------------------------------
  // use Laguerre method to find roots of the
  // denominator polynomial -- these roots are the
  // poles of the filter

  epsilon = 1.0e-6;
  epsilon2 = 1.0e-6;
  max_iter = 10;

  for(i=0; i<=filt_order; i++) work_coeff[i] = denom_poly[i];

  int i_stop;
  int biquad_cnt=0;
  if(Filt_Order%2)
    { //odd
    Num_Biquad_Sects = (Filt_Order-1)/2;
    i_stop = 1;
    }
  else
    { // even
    Num_Biquad_Sects = Filt_Order/2;
    i_stop = 2;
    }

  B0_Coef = new double[Num_Biquad_Sects];
  B1_Coef = new double[Num_Biquad_Sects];
  //Biquad_Coef_C = new double[(Filt_Order + (Filt_Order%2))/2];
  ///for(i=Filt_Order; i>1; i--)
  for(i=Filt_Order; i>i_stop; i-=2)
    {
    root = std::complex<double>(0.0,0.0);
    work_order = i;
    work_poly = new CmplxPolynomial( work_order, work_coeff );
    laguerre_status = LaguerreMethod( work_poly,
                                      &root,
                                      epsilon,
                                      epsilon2,
                                      max_iter);
    delete work_poly;

    #ifdef _DEBUG
    *DebugFile << "laguerre_status = "
              << laguerre_status << endl;
    #endif

    if(laguerre_status <0)
      {
      #ifdef _DEBUG
      *DebugFile << "FATAL ERROR - \n"
                << "Laguerre method failed to converge.\n"
                << "Unable to find poles for desired Bessel filter." 
                << endl;
      #endif
      exit(-1);
      }

    #ifdef _DEBUG
    *DebugFile << "root = ( " << root.real() << ", " << root.imag() << ")" << endl;
    #endif

    //--------------------------------------------
    // if imaginary part of root is very small
    // relative to real part, set it to zero

    if(fabs( root.imag() ) < epsilon*fabs( root.real() ))
      {
      root = std::complex<double>( root.real(), 0.0);
      }
//    Pole_Locs[filt_order+1-i] = root;
    Pole_Locs[filt_order-i] = root;
    Pole_Locs[filt_order+1-i] = std::conj(root);

    //Biquad_Coef_A[biquad_cnt] = 1.0;
    B1_Coef[biquad_cnt] = -2.0*root.real();
    B0_Coef[biquad_cnt] = std::norm(root);
    biquad_cnt++;

    //---------------------------------------------
    // deflate working polynomial by removing 
    // (s - r) factor where r is newly found root

    work1 = work_coeff[i];
    for(ii=i-1; ii>=0; ii--)
      {
      work2 = work_coeff[ii];
      work_coeff[ii] = work1;
      work1 = work2 + root * work1;
      }
    work1 = work_coeff[i-1];
    for(ii=i-2; ii>=0; ii--)
      {
      work2 = work_coeff[ii];
      work_coeff[ii] = work1;
      work1 = work2 + std::conj(root) * work1;
      }
    } // end of loop over i
  #ifdef _DEBUG
  *DebugFile << "work_coeff[1] = ( " << work_coeff[1].real() 
            << ", " << work_coeff[1].imag() << ")" << endl;
  *DebugFile << "work_coeff[0] = ( " << work_coeff[0].real() 
            << ", " << work_coeff[0].imag() << ")" << endl;
  #endif
  
  if(filt_order%2)
    { // order is odd
    Pole_Locs[filt_order-1] = -work_coeff[0];
    Real_Pole = -(work_coeff[0].real());
    }
  else
    { // order is even
    // the remaining unfactored polynomial should
    // be a quadratic with real-valued coefficients
    // use these coefficients as-is for biquad and solve
    // for complex conjugate roots to get last twp poles
    double b_work = work_coeff[1].real();
    double c_work = work_coeff[0].real();

    root = std::complex<double>(-b_work/2.0, sqrt(4.0*c_work - b_work*b_work)/2.0);
    Pole_Locs[filt_order-2] = root;
    Pole_Locs[filt_order-1] = std::conj(root);

    //Biquad_Coef_A[biquad_cnt] = 1.0;
    B1_Coef[biquad_cnt] = -2.0*root.real();
    B0_Coef[biquad_cnt] = std::norm(root);
    biquad_cnt++;
    #ifdef _DEBUG
    *DebugFile << "work_coeff[2] = ( " << work_coeff[2].real() 
              << ", " << work_coeff[2].imag() << ")" << endl;
    *DebugFile << "work_coeff[1] = ( " << work_coeff[1].real() 
              << ", " << work_coeff[1].imag() << ")" << endl;
    *DebugFile << "work_coeff[0] = ( " << work_coeff[0].real() 
              << ", " << work_coeff[0].imag() << ")" << endl;
    #endif
    }

  #ifdef _DEBUG
  *DebugFile << "pole[" << filt_order-1 << "] = ( "
            << Pole_Locs[filt_order-1].real() << ", " 
            << Pole_Locs[filt_order-1].imag() << ")" 
            << endl;
  #endif
  //----------------------------------------------
  // sort poles so that imaginary parts are in
  // ascending order.  This order is critical for
  // sucessful operation of ImpulseResponse().

  #ifdef _DEBUG
  for(i=0; i<filt_order; i++)
    {
    *DebugFile << "pole[" << i << "] = (" 
              << Pole_Locs[i].real() << ", "
              << Pole_Locs[i].imag() << ")" << endl;
    }
  *DebugFile << endl;
  #endif

  for(i=0; i<filt_order-1; i++)
    {
    smallest = Pole_Locs[i].imag();
    for( ii=i+1; ii<filt_order; ii++)
      {
      if(smallest <= Pole_Locs[ii].imag()) continue;
        work1 = Pole_Locs[ii];
        Pole_Locs[ii] = Pole_Locs[i];
        Pole_Locs[i] = work1;
        smallest = work1.imag();
      }
    }

  #ifdef _DEBUG
  for(i=0; i<filt_order; i++)
    {
    *DebugFile << "pole[" << i << "] = (" 
              << Pole_Locs[i].real() << ", "
              << Pole_Locs[i].imag() << ")" << endl;
    }
  #endif

  return;
};
//==============================================
BesselPrototype::~BesselPrototype(void)
{
}
