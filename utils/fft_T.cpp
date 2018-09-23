//
//  File = dc_fft.cpp
//

#include <iostream> 
#include <fstream>
#include <complex>

#include "dit_pino_T.h"
#include "fft_T.h"
#include "cbitrev_T.h"
using namespace std;
//======================================================
template< class T>
void IFFT(  complex<T>* sample_spectrum,
            complex<T>* time_signal,
            int num_samps )
{
    int i;
    for(i=0; i<num_samps; i++){
        time_signal[i] = sample_spectrum[i];
    }
    ComplexBitReverse<T>(time_signal, num_samps);
    IfftDitPino<T>(time_signal, num_samps);
    return;
}
//======================================================
template <class T>
void FFT(   complex<T>* time_signal,
            complex<T>* sample_spectrum,
            int num_samps )
{
    int i;
    for(i=0; i<num_samps; i++){
        sample_spectrum[i] = time_signal[i];
    }
    ComplexBitReverse<T>(sample_spectrum,num_samps);
    FftDitPino<T>(sample_spectrum, num_samps);
    return;
}
//======================================================
template <class T>
void FFT(   complex<double>* time_signal,
            complex<T>* sample_spectrum,
            int num_samps,
            int fft_len )
{
    int i;
    for(i=0; i<num_samps; i++){
        sample_spectrum[i] = time_signal[i];
    }
    for(i=num_samps; i<fft_len; i++){
        sample_spectrum[i] = float_complex(0.0,0.0);
    }

#ifdef _FFT_SIGNAL_DUMP
    ofstream time_sig_file("wind_sig.txt", ios::out);
    for(i=0; i<fft_len; i++){
        time_sig_file << i << ", " << real(sample_spectrum[i]) << endl;
    }
    time_sig_file.close();
#endif

    ComplexBitReverse<T>(sample_spectrum, fft_len);
    FftDitPino<T>(sample_spectrum, fft_len);
    return;
}
//======================================================
template <class T>
void FFT(   complex<float>* time_signal,
            complex<T>* sample_spectrum,
            int num_samps,
            int fft_len )
{
    int i;
    for(i=0; i<num_samps; i++){
        sample_spectrum[i] = time_signal[i];
    }
    for(i=num_samps; i<fft_len; i++){
        sample_spectrum[i] = complex<T>(0.0,0.0);
    }

#ifdef _FFT_SIGNAL_DUMP
    ofstream time_sig_file("wind_sig.txt", ios::out);
    for(i=0; i<fft_len; i++){
        time_sig_file << i << ", " 
                      << real(sample_spectrum[i]) << endl;
    }
    time_sig_file.close();
#endif

    ComplexBitReverse<T>(sample_spectrum, fft_len);
    FftDitPino<T>(sample_spectrum, fft_len);
    return;
}
//======================================================
template <class T>
void FFT(   double* time_signal,
            complex<T>* sample_spectrum,
            int num_samps,
            int fft_len )
{
    int i;
    for(i=0; i<num_samps; i++){
        sample_spectrum[i] = 
                         complex<T>(time_signal[i],0.0);
    }
    for(i=num_samps; i<fft_len; i++){
        sample_spectrum[i] = complex<T>(0.0,0.0);
    }

#ifdef _FFT_SIGNAL_DUMP
    ofstream time_sig_file("wind_sig.txt", ios::out);
    for(i=0; i<fft_len; i++){
        time_sig_file << i << ", " 
                    << real(sample_spectrum[i]) << endl;
    }
    time_sig_file.close();
#endif

    ComplexBitReverse<T>(sample_spectrum, fft_len);
    FftDitPino<T>(sample_spectrum, fft_len);
    return;
}
//======================================================
template <class T>
void FFT(   float* time_signal,
            complex<T>* sample_spectrum,
            int num_samps,
            int fft_len )
{
    int i;
    for(i=0; i<num_samps; i++) {
        sample_spectrum[i] = complex<T>(time_signal[i],0.0);
    }
    for(i=num_samps; i<fft_len; i++) {
        sample_spectrum[i] = complex<T>(0.0,0.0);
    }
#ifdef _FFT_SIGNSL_DUMP
    ofstream time_sig_file("wind_sig.txt", ios::out);
    for(i=0; i<fft_len; i++) {
        time_sig_file << i << ", " << real(sample_spectrum[i]) << endl;
    }
    time_sig_file.close();
#endif
    ComplexBitReverse<T>(sample_spectrum, fft_len);
    FftDitPino<T>(sample_spectrum, fft_len);
    return;
}
//======================================================
template <class T> 
void FFT( complex<T>* signal,
          int num_samps )
{
 ComplexBitReverse<T>(signal, num_samps);
 FftDitPino<T>(signal, num_samps);
 return;
}
//======================================================
template void IFFT<double>( std::complex<double>* sample_spectrum,
                    std::complex<double>* time_signal,
                    int num_samps );
template void FFT<double>( std::complex<double>* time_signal,
                    std::complex<double>* sample_spectrum,
                    int num_samps );
template void FFT<double>( std::complex<float>* time_signal,
                            std::complex<double>* sample_spectrum,
                            int num_samps,
                            int fft_len );
template void FFT<double>( double* time_signal,
                            std::complex<double>* sample_spectrum,
                            int num_samps,
                            int fft_len );
template void FFT<double>( float* time_signal,
                            std::complex<double>* sample_spectrum,
                            int num_samps,
                            int fft_len );

