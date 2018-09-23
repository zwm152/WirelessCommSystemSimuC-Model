 //
 //  File = levin.cpp
 //

//#include "complex.h"
#include <fstream>
#include "levin.h"
#include "overload.h"
using namespace std;

#ifdef _DEBUG
   extern ofstream *DebugFile;
#endif

 int LevinsonRecursion( double *toeplitz,
                        int ar_order,
                        double *a_vector,
                        double *driving_variance)
 {
   double sum, work, temp;
   double drv_var;
   int j, k, k_over_2, k_minus_j;

   drv_var = toeplitz[0];
   a_vector[0] = 1.0;
   for(k=1; k<=ar_order; k++) a_vector[k]=0.0;

   for(k=1; k<=ar_order; k++){
      sum = toeplitz[k];
      for(j=1; j<k; j++){
         sum += a_vector[j] * toeplitz[k-j];
      }
      work = -sum/drv_var;
      drv_var *= (1.0 - mag_sqrd(work));

      a_vector[k] = work;
      k_over_2 = k/2;
      for(j=1; j<=k_over_2; j++){
         k_minus_j = k-j;
         temp = a_vector[j];
         a_vector[j] = temp + work*conj(a_vector[k_minus_j]);
         if(j!=k_minus_j){
            a_vector[k_minus_j] += work * conj(temp);
         }
      }
   } 
   *driving_variance = drv_var;
   return(0);
 };