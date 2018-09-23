//
//  File = filter_proto.h
//

#ifndef _FILTER_PROTO_H_
#define _FILTER_PROTO_H_

#include "realpoly.h"


class AnalogFilterPrototype
{
public:
   AnalogFilterPrototype( void );

   AnalogFilterPrototype( int order );

   virtual ~AnalogFilterPrototype();

   std::complex<double> *GetZeros();

   std::complex<double> *GetPoles();

   int GetOrder();

   int GetNumZeros();

   int GetNumPoles();

   double GetHZero();

   int Get_Biquads(  double **coef_a2,
                 double **coef_a1,
                 double **coef_a0,
                 double **coef_b1,
                 double **coef_b0 );

   void DumpBiquads( ofstream* output_stream);
   RealPolynomial GetDenomPoly( bool binomial_use_enab );
   RealPolynomial GetNumerPoly( bool binomial_use_enab );
   void FilterFrequencyResponse(void);

protected:

   int Filt_Order;
   int Num_Poles;

   int Num_Zeros;

   std::complex<double> *Pole_Locs;
   std::complex<double> *Zero_Locs;

   double H_Zero;
   int Degree_Of_Denom;
   int Degree_Of_Numer;
   int Num_Biquad_Sects;
   double *A0_Coef;
   double *A1_Coef;
   double *A2_Coef;
   double *B0_Coef;
   double *B1_Coef;
   double Real_Pole;
   RealPolynomial Denom_Poly;
   RealPolynomial Numer_Poly;

};

#endif
