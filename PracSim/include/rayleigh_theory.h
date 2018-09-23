//
//  File = rayleigh_theory.h
//

#ifndef _RAYLEIGH_THEORY_H_ 
#define _RAYLEIGH_THEORY_H_

void RayleighPdf( double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename);

void RayleighCdf( double sigma,
               int num_pts,
               double pts_per_sigma,
               char *out_filename);

#endif
