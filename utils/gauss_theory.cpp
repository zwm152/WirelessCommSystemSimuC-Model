//
//  File = gauss_theory.cpp
//

#include <stdlib.h>
#include <math.h>
#include "gauss_theory.h"
#include "sinc.h"
#include "q_func.h"
#include <iostream>
#include <fstream>
#ifndef PI
  #define PI 3.1415926535897932
#endif
using namespace std;

void GaussPdf( double mean,
               double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename)
{
   double abscissa, ordinate;
   int n_end;
   ofstream *out_file;
   out_file = new ofstream(out_filename, ios::out);
   if(num_pts%2 == 0) num_pts++;
   n_end = (num_pts-1)/2;

   for(int n=-n_end; n<=n_end; n++)
   {
      abscissa = n*sigma/pts_per_sigma;
      ordinate = exp(-(abscissa-mean)*(abscissa-mean)/(2*sigma*sigma))/(sigma*sqrt(2.0*PI));
      (*out_file) << abscissa << ", " << ordinate << endl;
   }
   out_file->close();
   delete out_file;
}

void GaussCdf( double mean,
               double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename)
{
   double abscissa, ordinate;
   int n_end;
   ofstream *out_file;
   out_file = new ofstream(out_filename, ios::out);
   if(num_pts%2 == 0) num_pts++;
   n_end = (num_pts-1)/2;
   double root_2 = sqrt(2.0);

   for(int n=-n_end; n<=n_end; n++)
   {
      abscissa = n*sigma/pts_per_sigma;
      if(abscissa >=0.0)
      {
         ordinate = 0.5*(2.0-erfc((abscissa-mean)/sigma/root_2));
      }
      else
      {
         ordinate = 0.5*erfc((mean-abscissa)/sigma/root_2);
      }
      (*out_file) << abscissa << ", " << ordinate << endl;
   }
   out_file->close();
   delete out_file;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
