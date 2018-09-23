//
//  File = filter_response.h
//

#ifndef _FILTER_RESPONSE_H_
#define _FILTER_RESPONSE_H_

#include <complex>
#include "floatsig.h"
#include "psmodel.h"

typedef enum {
  LOWPASS_FILT_CONFIG,
  HIGHPASS_FILT_CONFIG,
  BANDPASS_FILT_CONFIG,
  BANDSTOP_FILT_CONFIG
  } FILT_RESP_CONFIG_T;


class FilterResponse
{
public:

  FilterResponse( int order );

  ~FilterResponse(void);

  std::complex<double> *GetProtoypeZeros( int *num_zeros );
  std::complex<double> *GetProtoypePoles( int *num_poles );
  double GetHZero(void);

  void LowpassDenorm( double cutoff_freq_nfu,
                      std::complex<double> **lowpass_pole_locs,
                      int *num_lowpass_poles,
                      std::complex<double> **lowpass_zero_locs,
                      int *num_lowpass_zeros,
                      double *h_zero );
  void FilterFrequencyResponse(void);

protected:

  char *Response_Name;
  
  int Filt_Order;
  
  std::complex<double> *Prototype_Pole_Locs;

  std::complex<double> *Prototype_Zero_Locs;

  double H_Zero;

  int Num_Prototype_Poles;

  int Num_Prototype_Zeros;

  std::complex<double> *Denorm_Pole_Locs;

  std::complex<double> *Denorm_Zero_Locs;

  int Num_Denorm_Poles;

  int Num_Denorm_Zeros;

  ofstream *ResponseFile;
    
};

#endif
