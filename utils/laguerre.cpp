//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = laguerre.cpp
//
//  Laguerre method for finding polynomial roots
//

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "laguerre.h"
using namespace std;

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

int LaguerreMethod( CmplxPolynomial *poly,
                    std::complex<double> *root_ptr,
                    double epsilon,
                    double epsilon2,
                    int max_iter)
{
int iter, j, order;
complex<double> p_eval, p_p, p_p_p;
complex<double> root, f, f_sq, g, rad;
complex<double> f_p_r, f_m_r, delta_root;
double old_delta_mag, root_mag, error;
complex<double> *coeff;
order = poly->GetDegree();
coeff = new complex<double>[order+1];
poly->CopyCoeffs(coeff);
root = *root_ptr;
old_delta_mag = 1000.0;

for(iter=1; iter<=max_iter; iter++)
  {
  p_p_p = complex<double>(0.0,0.0);
  p_p = complex<double>(0.0,0.0);
  p_eval = coeff[order];
  error = std::abs<double>(p_eval);
  root_mag = std::abs<double>(root);

  for( j=order-1; j>=0; j--)
    {
    p_p_p = p_p + root * p_p_p;
    p_p = p_eval + root * p_p;
    p_eval = coeff[j] + root * p_eval;
    error = std::abs<double>(p_eval) + root_mag * error;
    }
  error = epsilon2 * error;
  p_p_p = 2.0 * p_p_p;

  if(std::abs<double>(p_eval) < error)
    {
    cout << "mag(p_eval) = " << std::abs<double>(p_eval) << "  error = "
         << error << endl;
    *root_ptr = root;
    delete [] coeff;
    return(1);
    }
  f = p_p/p_eval;
  f_sq = f * f;
  g = f_sq - p_p_p/p_eval;
  rad = double(order-1)*(double(order) * g - f_sq);
  rad = std::sqrt<double>(rad);
  f_p_r = f + rad;
  f_m_r = f - rad;
  if( std::abs<double>(f_p_r) > std::abs<double>(f_m_r) )
    {
    delta_root = std::complex<double>(double(order), 0.0) / f_p_r;
    }
  else
    {
    delta_root = std::complex<double>(double(order), 0.0) / f_m_r;
    }
  root = root - delta_root;
  if( (iter > 6) && (std::abs<double>(delta_root) > old_delta_mag) )
    {
    *root_ptr = root;
    delete [] coeff;
    return(2);
    }
  if( std::abs<double>(delta_root) < (epsilon * std::abs<double>(root)))
    {
    *root_ptr = root;
    delete [] coeff;
    return(3);
    }
  old_delta_mag = std::abs<double>(delta_root);
  }
#ifdef _DEBUG
  *DebugFile << "Laguerre method failed to converge" << endl;
#endif
delete [] coeff;
return(-1);
}
