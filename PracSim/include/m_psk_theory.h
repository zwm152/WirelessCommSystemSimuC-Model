//
//  File = m_psk_theory.h
//

#ifndef _M_PSK_THEORY_H_ 
#define _M_PSK_THEORY_H_

void M_PskPsd( int big_m,
               double carrier_freq,
              double symb_energy,
              double symb_rate,
              double beg_freq,
              double end_freq,
              int num_pts,
              bool plot_in_db,
              char* out_filename);

void M_PskBer( int big_m,
               double beg_ebno,
              double end_ebno,
              int num_pts,
              char* out_filename);

#endif
