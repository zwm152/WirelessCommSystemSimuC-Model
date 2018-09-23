//
//  File = nrz_theory.h
//

#ifndef _NRZ_THEORY_H_ 
#define _NRZ_THEORY_H_

void NrzPsd(  double bit_energy,
              double bit_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void BiphasePsd(  double bit_energy,
              double bit_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void MillerPsd(  double bit_energy,
              double bit_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

//void BpskBer( double beg_ebno,
//              double end_ebno,
//              int num_pts,
//              char* out_filename);

#endif
