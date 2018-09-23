//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = elliptical_proto.cpp
//
//  Prototype Elliptical Filter
//

#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "elliptical_proto.h"
#include "ipow.h"
#include "filter_types.h"
extern ParmFile ParmInput;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

#define UPPER_SUMMATION_LIMIT 5

//======================================================
//  constructor

EllipticalPrototype::EllipticalPrototype( int filt_order,
                                          double passband_ripple,
                                          double stopband_ripple,
                                          double little_k)
                  :AnalogFilterPrototype( filt_order )
{
  double x, u, u4;
  double term, sum;
  double vv, ww, mu;
  double little_q;
  double big_D;
  int min_order;
  int i, m, r;
  double numer, denom;
  double p_sub_zero;
  double xx, yy;
  double aa, bb, cc;
  std::complex<double> work;


  //-------------------------------------------------
  //  check viability of parameter set


  x = pow( (1.0-little_k*little_k), 0.25);
  u = (1.0 -x)/(2.0*(1.0+x));
  u4 = u*u*u*u;

  little_q = u*(1.+(2*u4*(1.+(7.5*u4*(1.+(10.*u4))))));
  #ifdef _DEBUG
    *DebugFile << "In EllipticalPrototype:" << endl;
    *DebugFile << "   little_q = " << little_q << endl;
  #endif

  big_D = (pow(10.0, stopband_ripple/10.0) - 1.0)/
                    (pow(10.0, passband_ripple/10.0) - 1.0);

  min_order = (int)ceil(log10(16.0*big_D)/
                        log10(1.0/little_q));
  #ifdef _DEBUG
  *DebugFile << "In elliptical filter minimum order is " << min_order << endl;
  #endif

  if(filt_order < min_order){
    *DebugFile << "Fatal Error -- " << endl;
    *DebugFile << "Specified performance cannot be acheived "
              << "with an elliptical filter of specified order."
              << endl;
    *DebugFile << "Filter order must be at least " << min_order
              << endl;
    exit(-1);
    }
  //---------------------------------------------------------
  // if filter is viable, generate prototype

  Num_Poles = Filt_Order;
  Pole_Locs = new std::complex<double>[Num_Poles];

  if(Filt_Order%2)
    Num_Zeros = Filt_Order-1;
  else
    Num_Zeros = Filt_Order;
  Zero_Locs = new std::complex<double>[Num_Zeros];

  //----------------------------------
  numer = pow(10.0, passband_ripple/20.0) + 1.0;

  vv = log(numer/(numer -2.0))/(2.*filt_order);
  #ifdef _DEBUG
    *DebugFile << "   vv = " << vv << endl;
  #endif

  //---------------------------------------
  sum = 0.0;
  for( m=0; m < UPPER_SUMMATION_LIMIT; m++){
    term = ipow(-1.0, m);
    term *= ipow(little_q, m*(m+1));
    term *= sinh((2.0*m + 1)*vv);
    sum += term;
    }
  numer = sum * sqrt(sqrt(little_q));

  sum = 0.0;
  for( m=1; m < UPPER_SUMMATION_LIMIT; m++){
    term = ipow(-1.0, m);
    term *= ipow(little_q, m*m);
    term *= cosh(2.0 * m * vv);
    sum += term;
    }
  p_sub_zero = fabs(numer/(0.5 + sum));
  #ifdef _DEBUG
    *DebugFile << "   p_sub_zero = " << p_sub_zero << endl;
  #endif

  //----------------------------------------

  ww = 1.0 + little_k * p_sub_zero * p_sub_zero;
  ww = sqrt(ww*(1.0 + p_sub_zero * p_sub_zero / little_k));
  #ifdef _DEBUG
    *DebugFile << "   ww = " << ww << endl;
  #endif

  //-----------------------------------------

  r = (filt_order-(filt_order%2))/2;

  B1_Coef = new double[r];
  B0_Coef = new double[r];
  A2_Coef = new double[r];
  A1_Coef = new double[r];
  A0_Coef = new double[r];

  //-------------------------------------------

  H_Zero = 1.0;

  for(i=1; i<=r; i++){
    if(filt_order%2){
      mu = i;
      }
    else{
      mu = i - 0.5;
      }
    //--------------------------------

    sum = 0.0;
    for(m=0; m < UPPER_SUMMATION_LIMIT; m++){
      term = ipow(-1.0, m);
      term *= ipow(little_q, m*(m+1));
      term *= sin( (2*m+1) * PI * mu / filt_order );
      sum += term;
      }
    numer = 2.0 * sum *sqrt(sqrt(little_q));

    //--------------------------------------

    sum = 0.0;
    for(m=1; m < UPPER_SUMMATION_LIMIT; m++){
      term = ipow(-1.0,m);
      term *= ipow(little_q, m*m);
      term *= cos( TWO_PI * m * mu / filt_order );
      sum += term;
      }
    xx = numer/(1.0 + 2.0 * sum);

    //---------------------------------------

    yy = 1.0 - little_k * xx * xx;
    yy = sqrt(yy * (1.0-(xx*xx/little_k)));

    //-------------------------------------

    aa = 1.0/(xx*xx);
    A2_Coef[Num_Biquad_Sects] = 1.0;
    A1_Coef[Num_Biquad_Sects] = 0.0;
    A0_Coef[Num_Biquad_Sects] = aa;

    #ifdef _DEBUG
      *DebugFile << "   i = " << i << ",  aa = " << aa << endl;
    #endif

    //--------------------------------------
    denom = 1.0 + ipow(p_sub_zero * xx, 2);
    bb = 2.0 * p_sub_zero * yy/denom;
    B1_Coef[Num_Biquad_Sects] = bb;

    #ifdef _DEBUG
      *DebugFile << "   i = " << i << ",  bb = " << bb << endl;
    #endif

    //------------------------------------
    denom = ipow(denom, 2);
    numer = ipow(p_sub_zero * yy, 2) + ipow(xx*ww, 2);
    cc = numer/denom;
    B0_Coef[Num_Biquad_Sects] = cc;

    #ifdef _DEBUG
      *DebugFile << "   i = " << i << ",  cc = " << cc << endl;
    #endif

    Num_Biquad_Sects++;

    //---------------------------------------
    //
    H_Zero *= (cc/aa);

    //----------------------------------------
    // compute pair of pole locations

    work = std::sqrt<double>(std::complex<double>(bb*bb - 4.0*cc, 0) );

    Pole_Locs[i-1] = (std::complex<double>(-bb,0)-work)/2.0;
    Pole_Locs[filt_order-i] = (std::complex<double>(-bb,0)+work)/2.0;
    #ifdef _DEBUG
      *DebugFile << "Pole[ " << (i-1) << " ] = ( " << Pole_Locs[i-1].real()
                << ", " << Pole_Locs[i-1].imag() << ")" << endl;
    #endif

    //--------------------------------------
    // compute pair of zero locations
    //

    Zero_Locs[i-1] = std::complex<double>(0.0, sqrt(aa) );
    Zero_Locs[Num_Zeros-i] = -Zero_Locs[i-1];
    #ifdef _DEBUG
      *DebugFile << "Zero[ " << (i-1) << " ] = ( " << Zero_Locs[i-1].real()
                << ", " << Zero_Locs[i-1].imag() << ")" << endl;
    #endif
    } //end of loop over i

  //--------------------------------------
  //  Finish up Ho

  if( filt_order%2 ){
    H_Zero *= p_sub_zero;
    #ifdef _DEBUG
      *DebugFile << "In Elliptiocal prototype, p_sub_zero = " << p_sub_zero << endl;
    #endif

    Pole_Locs[(filt_order-1)/2] = std::complex<double>(-p_sub_zero, 0.0);
    Real_Pole = p_sub_zero;
    }
  else{
    H_Zero *= pow(10.0, passband_ripple/(-20.0));
    }
  #ifdef _DEBUG
    *DebugFile << "   H_Zero = " << H_Zero << endl;
  #endif


  return;
}
//==========================================================
EllipticalPrototype::~EllipticalPrototype()
{
}
