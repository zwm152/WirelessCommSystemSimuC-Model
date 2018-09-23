//
//  File = rayleigh_theory.cpp
//

#include <stdlib.h>
#include <math.h>
#include "rayleigh_theory.h"
#include "sinc.h"
#include "q_func.h"
#include <fstream>
#ifndef PI
  #define PI 3.1415926535897932
#endif
using namespace std;

void RayleighPdf( double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename)
{
   double abscissa, ordinate, b_sqrd;
   //b_sqrd = 2*sigma*sigma/(4.0-PI);
   b_sqrd = sigma*sigma;
   ofstream *out_file;
   out_file = new ofstream(out_filename, ios::out);
   //n_end = (num_pts-1)/2;

   for(int n=0; n<num_pts; n++)
   {
      abscissa = n*sigma/pts_per_sigma;
      ordinate = abscissa*exp(-abscissa*abscissa/(2*b_sqrd))/b_sqrd;
      (*out_file) << abscissa << ", " << ordinate << endl;
   }
   out_file->close();
   delete out_file;
}

void RayleighCdf( double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename)
{
   double abscissa, ordinate;
   //int n_end;
   ofstream *out_file;
   out_file = new ofstream(out_filename, ios::out);
   //if(num_pts%2 == 0) num_pts++;
   //n_end = (num_pts-1)/2;
   //double root_2 = sqrt(2.0);
   double two_b_sqrd;
   two_b_sqrd = 2.0 * sigma * sigma;

   for(int n=0; n<num_pts; n++)
   {
      abscissa = n*sigma/pts_per_sigma;
      ordinate = 1.0 - exp(-abscissa*abscissa/two_b_sqrd);
      (*out_file) << abscissa << ", " << ordinate << endl;
   }
   out_file->close();
   delete out_file;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
