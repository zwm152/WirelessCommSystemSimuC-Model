//
//  File = fsk_theory.h
//

#ifndef _FSK_THEORY_H_ 
#define _FSK_THEORY_H_

void FskCorrCoeff( double bit_intvl,
                    double beg_freq,
                    double end_freq,
                    int num_pts,
                    char* out_filename);

void FskPsd( double carrier_freq,
              double symb_energy,
              double symb_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void FskBer( double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename);

#endif
