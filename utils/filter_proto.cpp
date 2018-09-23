//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  File = filter_proto.cpp
//
//  Base class for all analog protoypes
//

#include <math.h>
#include "misdefs.h"
#include "parmfile.h"
#include "filter_proto.h"
#include "filter_types.h"
#include "unwrap.h"
extern ParmFile ParmInput;

extern ofstream *DebugFile;

//======================================================
//  constructor

AnalogFilterPrototype::AnalogFilterPrototype( void )
{
  Degree_Of_Denom = -1;
  Degree_Of_Numer = -1;
  Num_Biquad_Sects = 0;
  H_Zero = 1.0;
}

//======================================================
//  constructor

AnalogFilterPrototype::AnalogFilterPrototype( int filt_order )
{
  Filt_Order = filt_order;
  Degree_Of_Denom = -1;
  Degree_Of_Numer = -1;
  Num_Biquad_Sects = 0;
  H_Zero = 1.0;
}

//=======================================================
//  destructor

AnalogFilterPrototype::~AnalogFilterPrototype()
{
  if( !(Pole_Locs == NULL) ) delete []Pole_Locs;
  if( !(Zero_Locs == NULL) ) delete []Zero_Locs;

}
//=================================================
int AnalogFilterPrototype::GetOrder()
{
  return(Filt_Order);
}
//=================================================
int AnalogFilterPrototype::GetNumPoles()
{
  return(Num_Poles);
}
//================================================
std::complex<double> *AnalogFilterPrototype::GetPoles()
{
  return(Pole_Locs);
}
//=================================================
int AnalogFilterPrototype::GetNumZeros()
{
  return(Num_Zeros);
}
//================================================
std::complex<double> *AnalogFilterPrototype::GetZeros()
{
  return(Zero_Locs);
}
//================================================
double AnalogFilterPrototype::GetHZero()
{
  return(H_Zero);
}
//================================================
int AnalogFilterPrototype::Get_Biquads(  double **coef_a2,
                                         double **coef_a1,
                                         double **coef_a0,
                                         double **coef_b1,
                                         double **coef_b0 )
{
  if(Num_Biquad_Sects <= 0)
    {
    }
  *coef_a2 = A2_Coef;
  *coef_a1 = A1_Coef;
  *coef_a0 = A0_Coef;
  *coef_b1 = B1_Coef;
  *coef_b0 = B0_Coef;

  return(Num_Biquad_Sects);
}
//===============================================================
//

void AnalogFilterPrototype::DumpBiquads( ofstream* output_stream)
{
 (*output_stream) << "\nBiquad Coefficients\n" << endl;
 
 for(int i=0; i<Num_Biquad_Sects; i++)
   {
    (*output_stream) << i << "    b1 = " << B1_Coef[i]
                        << "    b0 = " << B0_Coef[i]
                        << endl;
//    (*output_stream) << i << ") a2 = " << A2_Coef[i]
//                        << "    a1 = " << A1_Coef[i]
//                        << "    a0 = " << A0_Coef[i]
//                        << "    b1 = " << B1_Coef[i]
//                        << "    b0 = " << B0_Coef[i]
//                        << endl;
   }
 if(Filt_Order%2)
  { // order is odd
  (*output_stream) << i << "    c = " << Real_Pole
                                      << endl;
  }                            
 return;
}
//===================================================
RealPolynomial AnalogFilterPrototype::GetDenomPoly(bool binomial_use_enab)
{
 //-----------------------------------------------------
 //  if denominator polynomial is not built yet, build
 //  it by multiplying together binomial factors (s-p[i]) 
 //  where the p[i] are the poles of the filter
 
 if(Degree_Of_Denom <0)
   {
   if(binomial_use_enab)
    { // use binomials
    *DebugFile << "using binomials\0" << endl;
    CmplxPolynomial cmplx_denom_poly =
                           CmplxPolynomial( std::complex<double>(1.0, 0.0),
                                            -Pole_Locs[0] );
    //                                        -Pole_Locs[1] );
    //for(int ii=2; ii<= Num_Poles; ii++)
    for(int ii=1; ii< Num_Poles; ii++)
      {
       cmplx_denom_poly *= CmplxPolynomial( std::complex<double>(1.0, 0.0),
                                            -Pole_Locs[ii] );
      }                                                                
    *DebugFile << "about to dump cmplx_denom_poly" << endl;
    cmplx_denom_poly.DumpToStream(&*DebugFile);
    
    Denom_Poly = RealPolynomial( cmplx_denom_poly);
    //Denom_Poly = poly_cast( cmplx_denom_poly);
    
    Degree_Of_Denom = Denom_Poly.GetDegree();
    
    *DebugFile << "\nin AnalogFilterPrototype::GetDenomPoly" << endl;
    *DebugFile << "(using binomials) real-valued version:" << endl;
    Denom_Poly.DumpToStream(&*DebugFile);
    }
  else
    { // use biquads

    *DebugFile << "using biquads\0" << endl;
    Denom_Poly = RealPolynomial( 1.0 );
   
    for(int ii=0; ii< Num_Biquad_Sects; ii++)
      {
       Denom_Poly *= RealPolynomial( 1.0, B1_Coef[ii], B0_Coef[ii] );
      }                                                                  
    Degree_Of_Denom = Denom_Poly.GetDegree();

    if(Filt_Order%2)
      { //odd order
      Denom_Poly *= RealPolynomial( 1.0, Real_Pole );
      }
    } // end of use biquds
   }                                    
  #ifdef _DEBUG
    
    *DebugFile << "\nin AnalogFilterPrototype::GetDenomPoly" << endl;
    *DebugFile << "denominator coefficients:" << endl;
    Denom_Poly.DumpToStream(&*DebugFile);
  #endif
   
 return(Denom_Poly);
}
//================================================================
//

RealPolynomial AnalogFilterPrototype::GetNumerPoly(bool binomial_use_enab)
{
 //---------------------------------------------------
 //  if numerator polynomial is not built yet,
 //  build it by multiplying together (s-z[i]) binomial
 //  factors where the z[i] are the zeros of the filter.
 
 if(Degree_Of_Numer <0)
   {
      if(binomial_use_enab)
      {
         CmplxPolynomial cmplx_poly =
                        CmplxPolynomial( d_complex_t( 1.0, 0.0), -Zero_Locs[0] );

         //for(int ii=2; ii<= Num_Zeros; ii++)
         for(int ii=1; ii< Num_Zeros; ii++)
         {
            cmplx_poly *= CmplxPolynomial( d_complex_t(1.0, 0.0), -Zero_Locs[ii] );
         }                                                          
         cmplx_poly.DumpToStream(&*DebugFile);

         Numer_Poly = RealPolynomial(cmplx_poly);

         Degree_Of_Numer = Numer_Poly.GetDegree();

         *DebugFile << "\nreal-valued version:" << endl;
         Numer_Poly.DumpToStream(&*DebugFile);
      }
      else
      {
      } 
   }                                  
 return(Numer_Poly);
}
//=========================================================
void AnalogFilterPrototype::FilterFrequencyResponse(void)
{
 std::complex<double> numer, denom;
 std::complex<double> transfer_function;
 std::complex<double> s_val, pole;
 double delta_freq, magnitude, phase;
 double peak_magnitude;
 double *mag_resp, *phase_resp, *group_dly;
 int i, k;

 delta_freq = 0.0025;
 //delta_freq = 0.0013889;
 peak_magnitude = -1000.0;

 ofstream* response_file = new ofstream("anlg_rsp.txt", ios::out);
 mag_resp = new double[800];
 phase_resp = new double[800];
 group_dly = new double[800];

 for( i=1; i<800; i++)
   {
    numer = std::complex<double>(1.0, 0.0);
    denom = std::complex<double>(1.0, 0.0);
    //s_val = double_complex(0.0, i*delta_freq*2.0*PI);
    s_val = std::complex<double>(0.0, i*delta_freq);

    //for( k=1; k<=Num_Zeros; k++)
    for( k=0; k<Num_Zeros; k++)
      {
       numer *= (s_val - Zero_Locs[k]);
      }

    //for( k=1; k<=Num_Poles; k++)
    for( k=0; k<Num_Poles; k++)
      {
       denom *= (s_val - Pole_Locs[k]);
      }
    transfer_function = numer/denom;
    magnitude = 10.0 * log10(std::norm(transfer_function));
    //magnitude = sqrt(std::norm(transfer_function));
    mag_resp[i] = magnitude;
    if(magnitude > peak_magnitude)
      {
       peak_magnitude = magnitude;
      }
    phase = 180.0 * std::arg(transfer_function)/PI;
    phase_resp[i] = phase;
   }
 UnwrapPhase(0, &(phase_resp[1]));
 for(i=2; i<800; i++)
  {
  UnwrapPhase(i, &(phase_resp[i]));
  }
 group_dly[1] = PI * (phase_resp[1] - phase_resp[2])
                / (180.0 * delta_freq);
 for(i=2; i<800; i++)
  {
  group_dly[i] = PI * (phase_resp[i-1] - phase_resp[i])
                 / (180.0 * delta_freq);
  }
 for(i=1; i<800; i++)
   {
    //(*response_file) << (1.0+ (i*delta_freq8)) << ",  "
    (*response_file) << i*delta_freq << ",  " << (mag_resp[i]-peak_magnitude)
                     << ",  " << phase_resp[i] 
                     << ",  " << group_dly[i] << endl;
                     //(*response_file)<< (1.0-(mag_resp[800-i]/peak_magnitude)) << endl;
   }
 //H_Sub_Zero = 1.0/sqrt(mag_sqrd_peak);
 response_file->close();
 delete []phase_resp;
 delete []mag_resp;
 return;
}

