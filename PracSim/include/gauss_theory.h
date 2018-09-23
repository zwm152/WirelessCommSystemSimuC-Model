//
//  File = gauss_theory.h
//

#ifndef _GAUSS_THEORY_H_ 
#define _GAUSS_THEORY_H_

void GaussPdf( double mean,
               double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename);

void GaussCdf( double mean,
               double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename);

#endif
