//
//  File = dump_spect.h
//

#ifndef _DUMP_SPECT_H_
#define _DUMP_SPECT_H_
#include <stdlib.h>
#include <fstream>
#include "math.h"
using namespace std;

void DumpSpectrum(  double *psd_est,
               int nsamps_in_psd,
               double delta_f,
               double freq_norm_factor,
               bool output_in_decibels,
               bool plot_two_sided,
               ofstream *psd_file);
#endif