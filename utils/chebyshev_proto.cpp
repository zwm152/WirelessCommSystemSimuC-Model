//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = chebyshev_proto.cpp
//
//  Prototype Chebyshev (Type I) Filter
//

#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "chebyshev_proto.h"
#include "filter_types.h"
extern ParmFile ParmInput;

#ifdef _DEBUG
extern ofstream *DebugFile;
#endif

//======================================================
//  constructor

ChebyshevPrototype::ChebyshevPrototype( int filt_order,
                                        double ripple,
                                        bool bw_is_ripple_bw )
                  :AnalogFilterPrototype( filt_order )
{
  Ripple = ripple;
  Bw_Is_Ripple_Bw = bw_is_ripple_bw;
  Bw_Is_3db_Bw = !bw_is_ripple_bw;
//=========================================================
//  Computes poles for normalized ChebyshevPrototype prototype

  int k;
  double x;
  double epsilon;
  double gamma;
  double sigma_mult;
  double omega_mult;
  double a_factor, r_factor;
  std::complex<double> work;

  Pole_Locs = new std::complex<double>[Filt_Order+1];
  Pole_Locs[0] = Filt_Order;
  Num_Poles = Filt_Order;
  Zero_Locs = new std::complex<double>[1];
  Zero_Locs[0] = 0;
  Num_Zeros = 0;

  epsilon = sqrt(pow(10.0, (Ripple/10.0))-1.0);

  if(Bw_Is_3db_Bw)
    {
    a_factor = log((1+sqrt(1.0-epsilon*epsilon))/epsilon)/Filt_Order;
    r_factor = cosh(a_factor);
    }

  gamma = pow( (1+sqrt(1.0 + epsilon*epsilon))/epsilon,
                1.0/double(Filt_Order));
  sigma_mult = ( (1.0/gamma) - gamma) / 2.0;
  omega_mult = ( (1.0/gamma) + gamma) / 2.0;

  for( k=1; k<=Filt_Order; k++)
    {
    x = PI * ((2*k)-1) / (2*Filt_Order);
    Pole_Locs[k-1] = std::complex<double>( sigma_mult*sin(x), omega_mult*cos(x));
    if(Bw_Is_3db_Bw) Pole_Locs[k-1] /= r_factor;

    }
  //-----------------------------------------
  //  Compute Biquads

  Num_Biquad_Sects = (Filt_Order-(Filt_Order%2))/2;

  //Biquad_Coef_A = new double[Num_Biquad_Sects];
  B1_Coef = new double[Num_Biquad_Sects];
  B0_Coef = new double[Num_Biquad_Sects];

  if(Filt_Order%2)
    { // order is odd
    Real_Pole = Pole_Locs[Num_Biquad_Sects].real();
    }

  for( k=1; k<=Num_Biquad_Sects; k++)
    {
    x = PI * ((2*k)-1) / (2*Filt_Order);
    //Biquad_Coef_A[k-1] = 1.0;
    B1_Coef[k-1] = -2.0 * sigma_mult * sin(x); 
    B0_Coef[k-1] = (0.25/(gamma*gamma)) +
                          gamma * gamma / 4.0 +
                          0.5 * cos(2.0 * x);
    }
  //-----------------------------------------
  //  Compute gain factor Ho

  work = 1.0;
  for( k=0; k<Filt_Order; k++)
    {
    work *= (-Pole_Locs[k]);
    }
  //H_Zero = std::real<double>(work);
  H_Zero = work.real();

  if(Filt_Order%2 == 0) // if order is even
    {
    H_Zero /= pow(10.0, Ripple/20.0);
    }
  //if(Bw_Is_3db_Bw)
  //  {
  //  H_Zero *= r_factor;
   // }

  #ifdef _DEBUG
  int i;
  for(i=0; i<filt_order; i++)
    {
    *DebugFile << "pole[" << i << "] = (" 
              << Pole_Locs[i].real() << ", "
              << Pole_Locs[i].imag() << ")" << endl;
    }
  #endif

  return;
}
//==========================================================
ChebyshevPrototype::~ChebyshevPrototype()
{
}