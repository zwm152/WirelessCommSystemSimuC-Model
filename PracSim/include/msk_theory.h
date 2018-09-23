//
//  File = msk_theory.h
//

#ifndef _MSK_THEORY_H_ 
#define _MSK_THEORY_H_

void MskPsd( double carrier_freq,
              double symb_energy,
              double symb_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void MskBer( double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename);

#endif
