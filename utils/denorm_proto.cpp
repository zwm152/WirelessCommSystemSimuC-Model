//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = denorm_proto.cpp
//
//  Converts a normalized lowpass prototype filter
//  into a denormalized LP, BP, BS, or HP filter.
//

#include <iostream> 
#include <fstream>
#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "denorm_proto.h"
#include "filter_types.h"
extern ParmFile ParmInput;

extern ofstream *DebugFile;

//======================================================
//  constructor

DenormalizedPrototype::DenormalizedPrototype( AnalogFilterPrototype *lowpass_proto_filt,
                                              FILT_BAND_CONFIG_T filt_band_config,
                                              double passband_edge,
                                              double passband_edge_2 )
                      :AnalogFilterPrototype()
{
  Filt_Order = lowpass_proto_filt->GetOrder();
  Filt_Band_Config = filt_band_config;
  Passband_Edge = passband_edge;
  Passband_Edge_2 = passband_edge_2;
  Lowpass_Proto_Filt = lowpass_proto_filt;

  switch (Filt_Band_Config) 
    {
    case LOWPASS_FILT_BAND_CONFIG:
      {
      LowpassDenormalize();
      break;
      }
    case BANDPASS_FILT_BAND_CONFIG:
      {
      BandpassDenormalize();
      break;
      }
    case HIGHPASS_FILT_BAND_CONFIG:
      {
      HighpassDenormalize();
      break;
      }
    case BANDSTOP_FILT_BAND_CONFIG:
      {
      BandstopDenormalize();
      break;
      }
    default:
      {
      cout << "Invalid Filter Band Config in DenormalizedPrototype" << endl;
      exit(0);
      }
    }// end of switch on Filt_Band_Config
}

//=======================================================
//  destructor

DenormalizedPrototype::~DenormalizedPrototype()
{
  if( !(Pole_Locs == NULL) ) delete []Pole_Locs;
  if( !(Zero_Locs == NULL) ) delete []Zero_Locs;

}
//================================================
void DenormalizedPrototype::LowpassDenormalize(void)
{
  double cutoff_freq;
  int j;
  std::complex<double> *prototype_poles;
  std::complex<double> *prototype_zeros;

  prototype_poles = Lowpass_Proto_Filt->GetPoles();
  prototype_zeros = Lowpass_Proto_Filt->GetZeros();
  Num_Poles = Lowpass_Proto_Filt->GetNumPoles();
  Num_Zeros = Lowpass_Proto_Filt->GetNumZeros();
  //H_Zero = 1.0;
  H_Zero = Lowpass_Proto_Filt->GetHZero();

  cutoff_freq = TWO_PI * Passband_Edge;

  Pole_Locs = new std::complex<double>[Num_Poles];
  for( j=0; j<Num_Poles; j++)
    {
    Pole_Locs[j] = prototype_poles[j] * cutoff_freq;
    //H_Zero *= std::abs<double>(Pole_Locs[j]);
    H_Zero *= cutoff_freq;
    #ifdef _DEBUG
      *DebugFile << "Denorm Pole_Locs[" << j << "] = (" 
                << std::real(Pole_Locs[j]) << ", "
                << std::imag(Pole_Locs[j]) << ")" << endl;
    #endif
    }
  #ifdef _DEBUG
    *DebugFile << "after poles, H_Zero = " << H_Zero << endl;
  #endif

  Zero_Locs = new std::complex<double>[Num_Zeros];
  for( j=0; j<Num_Zeros; j++)
    {
    Zero_Locs[j] = prototype_zeros[j] * cutoff_freq;
    //H_Zero /= std::abs<double>(Zero_Locs[j]);
    H_Zero /= cutoff_freq;
    #ifdef _DEBUG
      *DebugFile << "Denorm Zero_Locs[" << j << "] = (" 
                << std::real(Zero_Locs[j]) << ", "
                << std::imag(Zero_Locs[j]) << ")" << endl;
    #endif
    }
  #ifdef _DEBUG
    *DebugFile << "after zeros, H_Zero = " << H_Zero << endl;
  #endif
}
//================================================
void DenormalizedPrototype::HighpassDenormalize(void)
{
  //double cutoff_freq;
  int j;
  int num_prototype_zeros;
  std::complex<double> *prototype_poles;
  std::complex<double> *prototype_zeros;
  std::complex<double> cutoff_freq;

  prototype_poles = Lowpass_Proto_Filt->GetPoles();
  prototype_zeros = Lowpass_Proto_Filt->GetZeros();
  Num_Poles = Lowpass_Proto_Filt->GetNumPoles();
  num_prototype_zeros = Lowpass_Proto_Filt->GetNumZeros();

  // highpass will have a zero at s=0 for each
  // prototype zero at infinity
  Num_Zeros = Num_Poles;
  H_Zero = 1.0;

  cutoff_freq = TWO_PI * Passband_Edge;

  //  Divide the cutoff frequency (in rad/s) by the normalized pole
  //  locations to give the desired highpass poles.  This moves the poles
  //  from the unti circle onto a circle of radius cutoff_freq.

  Pole_Locs = new std::complex<double>[Num_Poles];
  for( j=0; j<Num_Poles; j++)
    {
    Pole_Locs[j] = cutoff_freq / prototype_poles[j];
    }

  // Map the finite zeros of prototype (if any) into HP locs
  Zero_Locs = new std::complex<double>[Num_Zeros];
  for( j=0; j<num_prototype_zeros; j++)
    {
    Zero_Locs[j] = cutoff_freq / prototype_zeros[j];
    }

  // Map the infinite zeros of prototype into HP zeros at s=0.
  for( j=num_prototype_zeros; j<Num_Zeros; j++)
    {
    Zero_Locs[j] = 0.0;
    }

}
//============================================================
// The lowpass to bandpass transformation implemented in this 
// function is based on Section 4.4.3 of "Filtering in the Time and
// Frequency Domains" by Herman J. Blinchikoff and Anatol I. Zverev
// (Wiley 1976).

void DenormalizedPrototype::BandpassDenormalize(void)
{
  //double cutoff_freq;
  int i;
//  int num_prototype_zeros;
  int num_prototype_poles;
  double omega_a, omega_b, omega_zero;
  double alpha, beta, gamma;
  double big_a, big_b;
  double little_a, little_b;
  double radical;
  double real_chunk, imag_chunk;
  int pole_num;
  int proto_pole_num;
  double sign;
  std::complex<double> *prototype_poles;
//  std::complex<double> *prototype_zeros;

  prototype_poles = Lowpass_Proto_Filt->GetPoles();
  //prototype_zeros = Lowpass_Proto_Filt->GetZeros();
  num_prototype_poles = Lowpass_Proto_Filt->GetNumPoles();
  Num_Poles = 2 * num_prototype_poles;
  Num_Zeros = num_prototype_poles;
  //num_prototype_zeros = Lowpass_Proto_Filt->GetNumZeros();
  H_Zero = 1.0;

  Pole_Locs = new std::complex<double>[Num_Poles];
  Zero_Locs = new std::complex<double>[Num_Zeros];
  for(i=0; i<Num_Zeros; i++)
    {
    Zero_Locs[i] = 0.0;
    }

  // convert critical frequencies from Hz to rad/sec
  //omega_b = TWO_PI * Passband_Edge_2; // flipped sense of omega_a/b on 11/30/00
  //omega_a = TWO_PI * Passband_Edge;
  omega_a = TWO_PI * Passband_Edge;
  omega_b = TWO_PI * Passband_Edge_2;

  pole_num = -1;
  // compute omega_zero as geometric mean of omega_a and omega_b
  omega_zero = sqrt(omega_a * omega_b);

  // compute the relative bandwidth, gamma
  gamma = (omega_b - omega_a)/omega_zero;

  // For each pole pair of the prototype we
  // will generate 2 pole pairs..
  // If num_prototype poles is odd, then we 
  // will calculate the last two poles which
  // are associated the real-valued prototype
  // pole later on.
  for(  proto_pole_num=1;
        proto_pole_num <= num_prototype_poles/2;
        proto_pole_num++)
    {
    *DebugFile << "working on proto pole " << proto_pole_num-1
              << " at ( " << prototype_poles[proto_pole_num-1].real()
              << ", " << prototype_poles[proto_pole_num-1].imag()
              << " )" << endl;
    //easter alpha = prototype_poles[proto_pole_num-1].real();
    alpha = -prototype_poles[proto_pole_num-1].real();

    beta = fabs(prototype_poles[proto_pole_num-1].imag());

    big_a = 1.0 - (( gamma * gamma / 4.0) *
                ( alpha * alpha - beta * beta ));
    big_b = -alpha * beta * gamma * gamma / 2.0;
    radical = sqrt( big_a * big_a + big_b * big_b );
    little_a = sqrt( ( radical + big_a ) / 2.0 );
    little_b = sqrt( ( radical - big_a ) / 2.0 );

    // for each prototype pair....
    sign = 1.0;
    for( i=0; i<2; i++)
      {
      real_chunk = omega_zero *
          ((-gamma * alpha / 2.0) + sign * little_b);
      imag_chunk = omega_zero *
          ((gamma * beta / 2.0) - sign * little_a);
          //((gamma * beta / 2.0) + sign * little_a);  // for alpha .lt. 0
          //((gamma * beta / 2.0) - sign * little_a);  // for alpha .gt. 0
      pole_num++;
      Pole_Locs[pole_num] = std::complex<double>(real_chunk, imag_chunk);
      #ifdef _DEBUG
        *DebugFile << "   made new pole at ( " << real_chunk << ", "
                  << imag_chunk << " ) " << endl;
      #endif
    
      pole_num++;
      Pole_Locs[pole_num] = std::complex<double>(real_chunk, (-imag_chunk) );
      #ifdef _DEBUG
        *DebugFile << "   made new pole at ( " << real_chunk << ", "
                  << (-imag_chunk) << " ) " << endl;
      #endif

      sign = -sign;

      }
    }
  //-------------------------------------------------------------
  //  if number of lowpass poles is odd, do special
  //  transformation for the pole on the real axis
  
  if( (num_prototype_poles%2) != 0)
    {
    //alpha = std::real<double>( prototype_poles[ num_prototype_poles/2+1 ] );
    //beta = std::imag<double>( prototype_poles[ num_prototype_poles/2+1 ] );
    alpha = std::real<double>( prototype_poles[ num_prototype_poles/2 ] );
    beta = std::imag<double>( prototype_poles[ num_prototype_poles/2 ] );

    big_a = 1.0 - gamma * gamma * alpha * alpha / 4.0;

    if( fabs(beta) > 0.0000001)
      {
      cout << "middle lowpass pole not on the real axis" << endl;
      cout << "pole at ( " << prototype_poles[ num_prototype_poles/2+1 ].real()
            << ", " << prototype_poles[ num_prototype_poles/2+1 ].imag()
            << " ) " << endl;
      exit(11);
      }
    if( big_a <= 0.0 )
      {
      cout << "error" << endl;
      exit(99);
      }
    else
      {
      real_chunk = alpha * (omega_b - omega_a) / 2.0;
      imag_chunk = omega_zero * sqrt(big_a);

      pole_num++;
      Pole_Locs[pole_num] = std::complex<double>( real_chunk, imag_chunk);

      pole_num++;
      Pole_Locs[pole_num] = std::complex<double>( real_chunk, (-imag_chunk) );
      }
    }
  // calculate H_Zero
  std::complex<double> cmpx_omega_zero;
  cmpx_omega_zero = std::complex<double>(0.0, omega_zero);
  for( i=0; i<Num_Poles; i++)
    {
    H_Zero *= std::abs<double>( Pole_Locs[i] - cmpx_omega_zero );
    }
  for( i=0; i<Num_Zeros; i++)
    {
    H_Zero /= std::abs<double>( Zero_Locs[i] - cmpx_omega_zero );
    }

}
//============================================================
// The lowpass to bandstop transformation implemented in this 
// function is based on Section 4.7.1 of "Filtering in the Time and
// Frequency Domains" by Herman J. Blinchikoff and Anatol I. Zverev
// (Wiley 1976).  Equation (4.7-4) was algebraically manipulated
// to obtain explict expressions for the BS pole pairs that are
// similar to the expressions provided for BP case in (4.4-31)

void DenormalizedPrototype::BandstopDenormalize(void)
{
  int i;
  int num_prototype_poles;
  int pole_num, proto_pole_num;
  double omega_a, omega_b, omega_zero;
  double alpha, beta, gamma;
  double big_a, big_b, big_c, big_c_sqrd;
  double radical, sign;
  double little_a, little_b;
  double real_chunk, imag_chunk;
  std::complex<double> *prototype_poles;

  prototype_poles = Lowpass_Proto_Filt->GetPoles();
  num_prototype_poles = Lowpass_Proto_Filt->GetNumPoles();
  Num_Poles = 2 * num_prototype_poles;
  Num_Zeros = 2 * num_prototype_poles;

  H_Zero = 1.0;

  Pole_Locs = new std::complex<double>[Num_Poles];
  Zero_Locs = new std::complex<double>[Num_Zeros];

  // convert critical frequencies from Hz to rad/sec
  omega_b = TWO_PI * Passband_Edge_2;
  omega_a = TWO_PI * Passband_Edge;

  pole_num = -1;

  // compute omega_zero as geometric mean of omega_a and omega_b
  omega_zero = sqrt(omega_a * omega_b);

  // put N zeros at +(j * omega_zero) and N zeros at -(j * omega_zero)
  for(i=0; i<Num_Zeros/2; i++)
    {
    Zero_Locs[2*i] = std::complex<double>(0.0, omega_zero);
    Zero_Locs[2*i+1] = std::complex<double>(0.0, -omega_zero);
    }

  // compute the relative bandwidth, gamma
  gamma = (omega_b - omega_a)/omega_zero;
  //gamma = (omega_a - omega_b)/omega_zero;

  // For each pole pair of the prototype we 
  // will generate 2 pole pairs..
  // If num_prototype poles is odd, then we
  // will calculate the last two poles which
  // are associated with the real-valued prototype
  // pole later on.

  for(  proto_pole_num = 1;
        proto_pole_num <= num_prototype_poles/2;
        proto_pole_num++)
    {
    #ifdef _DEBUG
      *DebugFile << "working on proto pole " << proto_pole_num-1
                << " at ( " << prototype_poles[proto_pole_num-1].real()
                << ", " << prototype_poles[proto_pole_num-1].imag()
                << " )" << endl;
    #endif
  
    alpha = -prototype_poles[proto_pole_num-1].real();
    beta = fabs(prototype_poles[proto_pole_num-1].imag());

    big_c = -gamma / (2.0 * ( alpha*alpha + beta*beta ));
    big_c_sqrd = big_c * big_c;
  
    big_a = 1.0 - (alpha * alpha * big_c_sqrd) + (beta * beta * big_c_sqrd);
    big_b = -2.0 * alpha * beta * big_c_sqrd;

    radical = sqrt( big_a * big_a + big_b * big_b );
    little_a = sqrt( ( radical + big_a ) / 2.0 );
    little_b = sqrt( ( radical - big_a ) / 2.0 );

    // for each prototype pair...
    sign = 1.0;
    for( i=0; i<2; i++)
      {
      real_chunk = omega_zero * (alpha * big_c + sign * little_b);
      imag_chunk = omega_zero * (beta * big_c + sign * little_a);
      pole_num++;

      Pole_Locs[pole_num] = std::complex<double>(real_chunk, imag_chunk);
      #ifdef _DEBUG
        *DebugFile << "   made new pole at ( " << real_chunk << ", "
                  << imag_chunk << " )" << endl;
      #endif

      pole_num++;
      Pole_Locs[pole_num] = std::complex<double>(real_chunk, (-imag_chunk));
      #ifdef _DEBUG
        *DebugFile << "   made new pole at ( " << real_chunk << ", "
                  << (-imag_chunk) << " )" << endl;
      #endif

      sign = -sign;
      }
    } // end of loop over prototype pole pairs
  //------------------------------------------------
  // if number of lowpass poles is odd, do special
  // transformation for the pole on the real axis

  if( (num_prototype_poles%2) != 0)
    {
    alpha = -prototype_poles[num_prototype_poles/2].real();
    beta = prototype_poles[num_prototype_poles/2].imag();
    big_a = 1.0 - gamma * gamma * alpha * alpha;

    if( fabs(beta) > 0.0000001)
      {
      cout << "middle lowpass pole not on the real axis" << endl;
      cout << "pole at ( " << prototype_poles[num_prototype_poles/2].real()
            << ", " << beta << " )" << endl;
      exit(11);
      }
    else
      {
      real_chunk = alpha;
      }
    }
  //
  // calculate H_Zero
}