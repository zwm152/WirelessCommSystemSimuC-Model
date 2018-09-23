//
//  File = fir_dsgn.cpp
//

#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <strstream>
#include "misdefs.h"   
#include "fir_dsgn.h"
#include "fir_resp_w_noise_bw.h"
#include "parmfile.h"
#include "sinc.h"
extern ParmFile *ParmInput;
extern ofstream DebugFile;

//=========================================

FirFilterDesign::FirFilterDesign( void )
{
 return;
} 

//===============================================

FirFilterDesign::FirFilterDesign( int num_taps )
{
 Num_Taps = num_taps;
 Imp_Resp_Coeff = new double[num_taps];
 Original_Coeff = new double[num_taps];
}


//==========================================================

FirFilterDesign::FirFilterDesign( int num_taps,
                                  double *imp_resp_coeff)
{
 Num_Taps = num_taps;
 Imp_Resp_Coeff = new double[num_taps];
 Original_Coeff = new double[num_taps];
 
 for(int n=0; n<num_taps; n++){
    Imp_Resp_Coeff[n] = imp_resp_coeff[n];
    Original_Coeff[n] = imp_resp_coeff[n];
   }
 return;
} 

//==========================================================

FirFilterDesign::FirFilterDesign( const char* instance_name)
{
  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Taps);

  Imp_Resp_Coeff = new double[Num_Taps];
  Original_Coeff = new double[Num_Taps];
  char work[10];
  work[0] = 'C';
  work[2] = 0;
 
  for(int n=0; n<Num_Taps; n++){
    work[1] = 65 + n;
    Imp_Resp_Coeff[n] = ParmInput->GetFloatParm(work);
    Original_Coeff[n] = Imp_Resp_Coeff[n];
    }
  return;
} 

//==========================================================

FirFilterDesign::FirFilterDesign( const char* instance_name,
                                  int type_of_response,
                                  double samp_intvl)
{
  OPEN_PARM_BLOCK;
  GET_INT_PARM(Num_Taps);
  GET_DOUBLE_PARM(Filter_Alpha);
  GET_DOUBLE_PARM(Symbol_Rate);
  double a, b, c, x;
  double epsilon = 0.000001;
  int n;

  Imp_Resp_Coeff = new double[Num_Taps];
  Original_Coeff = new double[Num_Taps];
 
  for( n=0; n <= (Num_Taps-1)/2; n++){
    switch(type_of_response){
      case 0: // raised cosine
        x = n * Symbol_Rate * samp_intvl;
        a = cos( x * PI * Filter_Alpha );
        b = 2.0 * Filter_Alpha * x;
        b = (1.0 - b*b);
        c = sinc(double(x*PI));
        if ( fabs(b) > epsilon){
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = a * c / b;
          }
        else{
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = 0;
          }
        break;
      case 1: // root raised cosine
        x = n * Symbol_Rate * samp_intvl;
        a = sin( x * PI * (1.0 - Filter_Alpha));
        b = 4.0 * x * Filter_Alpha;
        b = x * PI * (1.0 - b*b);
        c = 4.0 * x * Filter_Alpha * cos( x * PI * (1.0 + Filter_Alpha));        
        if ( fabs(b) > epsilon){
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = (a + c) / b;
          }
        else if(n==0){ // t == 0
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = 1.0 - Filter_Alpha + 4.0*Filter_Alpha/PI;
          }
        else{
          x = PI/(4.0 * Filter_Alpha);
          a = (1.0 + 2.0/PI) * sin(x);
          c = (1.0 - 2.0/PI) * cos(x);
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = Filter_Alpha * (a+c) / sqrt(2.0);
          }
        break;
      case 2: // pure delay
        if(n==0)
          {
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = 1.0;
          }
        else
          {
          Imp_Resp_Coeff[n + (Num_Taps-1)/2] = 0.0;
          }
        break;

      }
    Imp_Resp_Coeff[ ((Num_Taps-1)/2) - n] = Imp_Resp_Coeff[n + (Num_Taps-1)/2];
    }
  for( n=0; n < Num_Taps; n++)
    {
    Original_Coeff[n] = Imp_Resp_Coeff[n];
    }
  return;
} 

//==========================================================
// constructor that allocates array of length num_taps 
// and initializes this array to values contained in 
// input array *imp_resp_coeff.  It is assumed that only
// half of the needed values are provided in the array.
// The remaining values are obtained from the given values
// in accordance with the type of symmetry indicated by the
// second argument. 
//----------------------------------------------------------
FirFilterDesign::FirFilterDesign( int num_taps,
                                  FIR_SYM_T symmetry,
                                  double *input_coeff)
{
 int last_left, first_right, n;
 Num_Taps = num_taps;
 Coeff_Symmetry = symmetry;

 Original_Coeff = new double[num_taps];
 Imp_Resp_Coeff = new double[num_taps];
 Quant_Coeff = new long[num_taps];
 if(num_taps%2)
   {  // odd length
   last_left = (num_taps-1)/2;
   first_right = last_left;
   }
 else
   {  // even length
   last_left = (num_taps-2)/2;
   first_right = last_left+1;
   }
 
 switch (symmetry) {
   case FIR_SYM_EVEN_LEFT:
     for(n=0; n<=last_left; n++)
       {
        Original_Coeff[n] = input_coeff[n];
        Imp_Resp_Coeff[n] = input_coeff[n];
        Quant_Coeff[n] = long(Original_Coeff[n]);
       }
     for(n=first_right; n<num_taps; n++)
       {
        Original_Coeff[n] = input_coeff[num_taps-n-1];
        Imp_Resp_Coeff[n] = input_coeff[num_taps-n-1];
        Quant_Coeff[n] = long(Original_Coeff[n]);
       }
    } // end of switch on symmetry
 return;
} 

//============================================== 
// method to allocate coefficient array 
// after default constructor has been used 
//----------------------------------------------

void FirFilterDesign::Initialize( int num_taps )
{
 Num_Taps = num_taps;
 Imp_Resp_Coeff = new double[num_taps];
 Original_Coeff = new double[num_taps];
}

//============================================================
//  method to quantize coefficients
//------------------------------------------------------------

void FirFilterDesign::QuantizeCoefficients( long quant_factor,
                                            bool rounding_enabled )
{
 int n;
 long work_long;
 
 //-----------------------------------
 // if quant_factor == 0, then restore
 // coefficients to their original,
 // unquantized values
 
 if( quant_factor == 0){
    for( n=0; n<Num_Taps; n++){
       Imp_Resp_Coeff[n] = Original_Coeff[n];
      }
    return;
   }

 //-------------------------------------------
 // quantize the original coefficient values
    
 for( n=0; n< Num_Taps; n++)
  {
   if(rounding_enabled)
     {work_long = long((quant_factor * Original_Coeff[n])+0.5);}
   else
     {work_long = long(quant_factor * Original_Coeff[n]);}
     
   Imp_Resp_Coeff[n] = double(work_long)/double(quant_factor);
  }
 return;
}       


//============================================================
//  method to scale coefficients
//------------------------------------------------------------

void FirFilterDesign::ScaleCoefficients( double scale_factor )
{
 int n;
 for( n=0; n< Num_Taps; n++)
  {
   Original_Coeff[n] = scale_factor * Original_Coeff[n];
   Imp_Resp_Coeff[n] = Original_Coeff[n];
  }
 return;
}       

//======================================================
//  scale coefficients so that magnitude response
//  has unity gain at passband peak
//------------------------------------------------------

void FirFilterDesign::NormalizeFilter( void )
{
  int n;
  double passband_peak;
  FirFilterResponse *temp_response;

  temp_response = new FirFilterResponse( this, 500, 0, 0, NULL);
  passband_peak = temp_response->GetIntervalPeak( 0, 499);
  delete temp_response;
  for( n=0; n< Num_Taps; n++)
  {
   Imp_Resp_Coeff[n] = Original_Coeff[n]/passband_peak;
  }
  return;
}

//======================================================
// copy coefficient values from array *Imp_Resp_Coeff
// to output array *coeff
//------------------------------------------------------
 
void FirFilterDesign::CopyCoefficients( double *coeff)
{
 for(int n=0; n<Num_Taps; n++)
   {
    coeff[n] = Imp_Resp_Coeff[n];
   }
 return;
}


void FirFilterDesign::CopyCoefficients( float *coeff)
{
 for(int n=0; n<Num_Taps; n++)
   {
    coeff[n] = float(Imp_Resp_Coeff[n]);
   }
 return;
}

void FirFilterDesign::CopyCoefficients( int *coeff)
{
 for(int n=0; n<Num_Taps; n++)
   {
    coeff[n] = int(Imp_Resp_Coeff[n]);
   }
 return;
}


//===================================
//  get number of filter taps
//-----------------------------------
 
int FirFilterDesign::GetNumTaps(void)
{
 return(Num_Taps);
}


//==============================================================
// dump complete set of coefficients to output_stream
//--------------------------------------------------------------

void FirFilterDesign::DumpCoefficients( ofstream* output_stream)
{ 
 //output_stream->setf(ios::scientific, ios::doublefield);
 output_stream->precision(8);
 for(int n=0; n<Num_Taps; n++)
   {
    //(*output_stream) << "h[" << n << "] = " 
    //                 << Imp_Resp_Coeff[n] << endl;
    (*output_stream) << n << ", " 
                     << Imp_Resp_Coeff[n] << endl;
   }
 output_stream->precision(0);
 output_stream->setf(0, ios::floatfield);
 return;
}


//==============================================
//  get pointer to coefficient array
//----------------------------------------------
double* FirFilterDesign::GetCoefficients(void)
{
 cout << "in fs_dsgn, Imp_Resp_Coeff = " << (void*)Imp_Resp_Coeff << endl;
 return(Imp_Resp_Coeff);
}

//========================================================
// apply discrete-time window to filter coefficients
//--------------------------------------------------------

void FirFilterDesign::ApplyWindow( GenericWindow *window)
{
 for(int n=0; n<Num_Taps; n++)
   {
    Imp_Resp_Coeff[n] *= window->GetDataWinCoeff(n);
    Original_Coeff[n] = Imp_Resp_Coeff[n];
   }
} 
//======================================================
// extract a subset of coefficient values for defining
// a polyphase filter 
//------------------------------------------------------
 
void FirFilterDesign::ExtractPolyphaseSet( double *coeff,
                                           int decim_rate,
                                           int rho)
{
 double *local_coeff;
 local_coeff = coeff;

 for(int n=rho; n<Num_Taps; n+=decim_rate)
   {
    *local_coeff++ = Imp_Resp_Coeff[n];
   }
 return;
}


