//
//  File = qam_theory.h
//

#ifndef _QAM_THEORY_H_ 
#define _QAM_THEORY_H_

void QamPsd( int big_m,
               double carrier_freq,
              double symb_energy,
              double symb_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void QamBer( int big_m,
               double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename);

#endif
