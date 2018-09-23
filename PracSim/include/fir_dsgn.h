//
//  File = fir_dsgn.h
//

#ifndef _FIR_DSGN_H_
#define _FIR_DSGN_H_ 

#include <fstream>
#include "gen_win.h"
#include "typedefs.h"

typedef enum {
  FIR_SYM_NONE,
  FIR_SYM_EVEN_LEFT,
  FIR_SYM_EVEN_RIGHT,
  FIR_SYM_ODD_LEFT,
  FIR_SYM_ODD_RIGHT
} FIR_SYM_T;


class FirFilterDesign
{
public:
  
  //---------------------
  // default constructor
    
  FirFilterDesign( );
  
  //-------------------------------------
  // constructor that allocates array of
  // length num_taps to hold coefficients 
  
  FirFilterDesign( int num_taps );
  
  //-------------------------------------
  // constructor that reads from ParmInput

  FirFilterDesign( const char* instance_name);
  //-------------------------------------
  // constructor that sets up coefficients
  // for a raised-cosine filter

  FirFilterDesign(  const char* instance_name,
                    int type_of_response,
                    double samp_rate );
  //-------------------------------------
  // constructor that allocates array of
  // length num_taps and initializes this
  // array to values contained in input 
  // array *imp_resp_coeff
  
  FirFilterDesign( int num_taps,
                   double *imp_resp_coeff );
  
  //-------------------------------------
  // constructor for filter with symmetric
  // impulse response.  Only half of the values
  // needed for initialization are provided
  // in the input array *imp_resp_coeff.
  // The remaining half are obtained from
  // the given half in accordance with the
  // specific type of symmetry indicated by
  // the second argument.

  FirFilterDesign( int num_taps,
                   FIR_SYM_T symmetry,
                   double *imp_resp_coeff );
  
  //------------------------------------------ 
  // allocate coefficient array *Imp_Resp_Coeff
  // after default constructor has been used
                   
  void Initialize( int num_taps );
  
  //-------------------------------------------
  //  method to quantize coefficients

  void QuantizeCoefficients( long quant_factor,
                             bool rounding_enabled );
 
  //-------------------------------------------
  //  method to scale coefficients

  void ScaleCoefficients( double scale_factor );
  
  void NormalizeFilter( void );
  //----------------------------------------
  // copy coefficients from input array
  // *coeff into array *Imp_Resp_Coeff 
  
  void CopyCoefficients( double *coeff);
  void CopyCoefficients( float *coeff);
  void CopyCoefficients( int *coeff);
  
  //----------------------------------------------
  // dump coefficient set to output_stream 
  
  void DumpCoefficients( ofstream* output_stream);
 
  //----------------------------------
  // get pointer to coefficient array 

  //this should be changed to new a copy and return
  // pointer to this copy
  
  double* GetCoefficients(void);
  
  //---------------------------
  // get number of filter taps
  
  int GetNumTaps(void);
  
  //---------------------------------------
  // apply discrete-time window
  // to filter coefficients
  
  void ApplyWindow( GenericWindow *window);
  
  void ExtractPolyphaseSet( double *coeff,
                            int decim_rate,
                            int rho);
  
  
protected:
  
  int Num_Taps;
  
  double* Imp_Resp_Coeff;
  double* Original_Coeff;
  long* Quant_Coeff;
  FIR_SYM_T Coeff_Symmetry;
  double Filter_Alpha;
  double Symbol_Rate;
};

#endif
