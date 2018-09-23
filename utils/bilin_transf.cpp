//
//  File = bilin_transf.cpp
//                     

 #include <stdlib.h> 
 #include <iostream>
 #include <fstream> 
 #include "misdefs.h"
 #include "typedefs.h" 
 #include <complex>
 #include "bilin_transf.h"
 extern ofstream *DebugFile;
 
//====================================================
//
//----------------------------------------------------
void BilinearTransf( DenormalizedPrototype* filt_proto,
                     double big_t,
                     double **a_ret,
                     double **b_ret)

{
  int max_poles;
  int j, m, n;
  int max_coeff;
  double little_h;
  complex<double> *mu;
  complex<double> alpha;
  complex<double> beta;
  complex<double> gamma;
  complex<double> delta;
  complex<double> eta;
  complex<double> work;
  complex<double> c2;
  complex<double> *pole;
  complex<double> *zero;

  double *a, *b;
  int num_poles, num_zeros;
  double h0;
  double adjust;

  pole = filt_proto->GetPoles();
  num_poles = filt_proto->GetNumPoles();
  zero = filt_proto->GetZeros();
  num_zeros = filt_proto->GetNumZeros();
  h0 = filt_proto->GetHZero();

  if(num_poles > num_zeros){
    max_poles = num_poles;
    }
  else{
    max_poles = num_zeros;
    }
  //-------------------------------------------
  a = new double[max_poles+1];
  *a_ret = a;
  b = new double[max_poles+1];
  *b_ret = b;
  
  mu = new std::complex<double>[max_poles+1];
  
  for(j=0; j<=max_poles; j++){
    mu[j] = 0.0;
    a[j] = 0.0;
    b[j] = 0.0;
    }
  //------------------------------------------
  little_h = 1.0;
  work = 1.0;
  c2 = 2.0;
  adjust = 1.0;

  for(n=0; n<num_poles; n++){
    work = work * (c2 - (big_t * pole[n]));
    little_h = little_h * big_t;
    adjust *= 4.0;
    }
  adjust *= num_poles;
  little_h = h0 * little_h / std::real(work);

  //-------------------------------------------------
  mu[0] = 1.0;

  for( n=1; n<=max_poles; n++){
    mu[n] = 0.0;
    }
  for( n=1; n<=num_poles; n++){
    gamma = complex<double>( 2.0/big_t, 0.0) - pole[n-1];
    delta = complex<double>( -2.0/big_t, 0.0) - pole[n-1];
    eta = delta/gamma;

    for( j=n; j>=1; j--){
      mu[j] = gamma * mu[j] + (delta * mu[j-1]);
      #ifdef DEBUG
        *DebugFile << "mu[" << j << "] = " << mu[j] << endl;
      #endif
      }
    mu[0] = gamma * mu[0];
    #ifdef DEBUG
      *DebugFile << "mu[0] = " << mu[0] << endl;
    #endif
    }
  for( j=1; j<=num_poles; j++){
    a[j] = -little_h * mu[j].real() / h0;
    }
  //--------------------------------------------
  mu[0] = 1.0;
  for( n=1; n<=max_poles; n++){
    mu[n] = 0.0;
    }
  max_coeff = 0;
  //--------------------------------------------
  for(m=1; m<=(num_poles-num_zeros); m++){
    max_coeff++;
    for( j=max_coeff; j>=1; j--){
      mu[j] = mu[j] + mu[j-1];
      }
    }
  for( m=1; m<=num_zeros; m++){
    max_coeff++;
    alpha = complex<double>( 2.0/big_t, 0.0) - zero[m-1];
    beta = complex<double>( -2.0/big_t, 0.0) - zero[m-1];

    for( j=max_coeff; j>=1; j--){
      mu[j] = alpha * mu[j] + (beta * mu[j-1]);
      }
    mu[0] = alpha * mu[0];
    }
  for(j=0; j<=max_coeff; j++){
    b[j] = little_h * mu[j].real();
    }
  return;
} 
