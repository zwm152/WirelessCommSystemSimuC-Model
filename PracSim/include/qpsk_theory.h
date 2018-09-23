//
//  File = qpsk_theory.h
//

#ifndef _QPSK_THEORY_H_ 
#define _QPSK_THEORY_H_

void QpskPsd( double carrier_freq,
              double symb_energy,
              double symb_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void QpskBer( double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename);

#endif
