//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = filtfunc.h
//
//

#ifndef _FILTFUNC_H_
#define _FILTFUNC_H_

//#include <fstream>
#include "poly_T.h"
//#include "typedefs.h"

class FilterTransFunc
{ 
 public: 
   
   FilterTransFunc( void );
   
   FilterTransFunc( int order);
   
   void FilterFrequencyResponse(void);

   std::complex<double>* GetPrototypePoles( int *num_poles );   
   
   std::complex<double>* GetPoles( int *num_poles ); 
   
   std::complex<double> GetPole(int pole_indx);  
   
   std::complex<double>* GetPrototypeZeros( int *num_zeros );
   
   std::complex<double>* GetZeros( int *num_zeros );

   std::complex<double> GetZero(int zero_indx);  
   
   void LowpassDenorm(double cutoff_freq_hz);
   
   int GetNumPoles(void);
   
   int GetNumZeros(void);              
   
   float GetHSubZero( void );
   
   void DumpBiquads( ofstream* output_stream);

   Polynomial GetDenomPoly( void );
 
   Polynomial GetNumerPoly( void );

   void FrequencyPrewarp( double sampling_interval );
 
 protected:
 
   int Filter_Order;
   int Num_Denorm_Poles;
   int Num_Denorm_Zeros;
   int Degree_Of_Denom;
   int Degree_Of_Numer;
   int Num_Prototype_Poles;
   int Num_Prototype_Zeros;
   int Num_Biquad_Sects;
   bool Filter_Is_Denormalized;
   //double Denorm_Cutoff_Freq_Hz;
   double Denorm_Cutoff_Freq_Rad;
   double *A_Biquad_Coef;
   double *B_Biquad_Coef;
   double *C_Biquad_Coef;
   double H_Sub_Zero;
   std::complex<double> *Prototype_Pole_Locs;
   std::complex<double> *Prototype_Zero_Locs;
   std::complex<double> *Denorm_Pole_Locs;
   std::complex<double> *Denorm_Zero_Locs;
   Polynomial Denom_Poly;
   Polynomial Numer_Poly;
   ofstream *Response_File;
};

#endif
