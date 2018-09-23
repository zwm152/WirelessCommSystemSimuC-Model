//
//  File = iir_comp_resp.cpp
//
//  Member functions for class IirComputedResponse
//

#include <math.h>
#include <stdlib.h>
#include <complex>
#include "iir_comp_resp.h"
#include "typedefs.h"
#include "misdefs.h"
#include "unwrap.h"
#ifdef _DEBUG
extern ofstream *DebugFile;
#endif
//============================================================
IirComputedResponse::IirComputedResponse( double *numer_coeff,
                                          double *denom_coeff,
                                          int filt_order,
                                          double samp_intvl,
                                          int num_resp_pts,
                                          bool db_scale_enabled )
{
 int resp_indx, tap_indx;
 double delta_freq;
 double theta, phi;
 int max_numer_idx, max_denom_idx;
 double real_sum, imag_sum;
 std::complex<double> numerator, denominator;
 
 #ifdef _DEBUG
  *DebugFile << " in ctor for IirComputedResponse" << endl;
 #endif

  Num_Resp_Pts = num_resp_pts;
  Db_Scale_Enabled = db_scale_enabled;
  Samp_Intvl = samp_intvl;

  Freq_Resp = new std::complex<double>[Num_Resp_Pts];
  Mag_Resp = new double[Num_Resp_Pts];
  Phase_Resp = new double[Num_Resp_Pts];

  delta_freq = PI/( samp_intvl * Num_Resp_Pts );
  theta = delta_freq * samp_intvl;
  max_numer_idx = filt_order;
  max_denom_idx = filt_order;

  for( resp_indx=1; resp_indx<Num_Resp_Pts; resp_indx++){
    real_sum = 0.0;
    imag_sum = 0.0;

    for( tap_indx=0; tap_indx<=max_numer_idx; tap_indx++){
      phi = theta * resp_indx * tap_indx;
      real_sum += (numer_coeff[tap_indx] * cos(phi));
      imag_sum -= numer_coeff[tap_indx] * sin(phi);
      }  
    numerator = std::complex<double>(real_sum, imag_sum);

    real_sum = 1.0;
    imag_sum = 0.0;
  
    for( tap_indx=1; tap_indx<=max_denom_idx; tap_indx++){
       phi = theta * tap_indx * resp_indx;
       real_sum -= (denom_coeff[tap_indx] * cos(phi));
       imag_sum += (denom_coeff[tap_indx] * sin(phi));
      } 
    denominator = std::complex<double>(real_sum, imag_sum); 
    Freq_Resp[resp_indx] = numerator/denominator;
    }
  Freq_Resp[0] = Freq_Resp[1];
   
 //-----------------------------------------------
 //  compute magnitude and phase of response

 for( resp_indx=0; resp_indx<Num_Resp_Pts; resp_indx++){
    Phase_Resp[resp_indx] = std::arg<double>(Freq_Resp[resp_indx]);

   if(Db_Scale_Enabled)
     {Mag_Resp[resp_indx] = 10.0 * log10(std::norm<double>(Freq_Resp[resp_indx]));}
   else
     {Mag_Resp[resp_indx] = std::abs<double>(Freq_Resp[resp_indx]);}
   }
 

 return;
}

//=======================================================

void IirComputedResponse::NormalizeResponse( void )
{
 int n;
 double peak;
 
 if(Db_Scale_Enabled){
    peak = -100.0; 
    
    for( n=0; n < Num_Resp_Pts; n++)
      {if(Mag_Resp[n]>peak) peak = Mag_Resp[n];}
    for( n=0; n < Num_Resp_Pts; n++)
      {Mag_Resp[n] = Mag_Resp[n] - peak;}
   }
 else
   {
    peak = 0.0;
    
    for( n=0; n < Num_Resp_Pts; n++)
      {if(Mag_Resp[n]>peak) peak = Mag_Resp[n];}
    for( n=0; n < Num_Resp_Pts; n++)
      {Mag_Resp[n] = Mag_Resp[n] / peak;}
   }
 return;
}
//===============================================

double* IirComputedResponse::GetMagResp( void)
{
 return(Mag_Resp);
}

//===========================================================

void IirComputedResponse::DumpMagResp( ofstream *resp_file )
{
 double freq, delta_freq, phase;
 
 delta_freq = 0.5/( Samp_Intvl * Num_Resp_Pts );

 for(int n=0; n<Num_Resp_Pts; n++){
    freq = n*delta_freq;
    phase = 180.0 * Phase_Resp[n] /PI;
    UnwrapPhase(n, &phase);
    (*resp_file) << freq << ", " 
                     << Mag_Resp[n] << ", "
                     << phase << endl;
   }
 return;
}

