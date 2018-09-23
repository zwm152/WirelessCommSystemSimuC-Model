//
//  File = fir_resp.h
//

#ifndef _FIR_RESP_H_
#define _FIR_RESP_H_

#include "fir_dsgn.h"

class FirFilterResponse
{
public:

  //-------------------------------------------------
  // constructor with all configuration parameters 
  // passed as input arguments

  FirFilterResponse( FirFilterDesign *filter_design,
                     int num_resp_pts,
                     int db_scale_enabled,
                     int normalize_enabled,
                     char* resp_file_name );
  
  //--------------------------------------------------
  //  alternate constructor with configuration 
  //  parameters obtained interactively through 
  //  streams uin and uout
                     
  FirFilterResponse( FirFilterDesign *filter_design,
                     istream& uin,
                     ostream& uout );
  
  //--------------------------------------
  // method to compute magnitude response
  // from the data set up by constructor
  
  double ComputeMagResp( void );
  
  //---------------------------------------
  // method to normalize magnitude response
  
  double NormalizeResponse( void );
  
  //-----------------------------
  // method that returns pointer
  // to an array that holds the
  // completed magnitude response
  // (ordinates only)
                         
  double* GetMagResp( void);
  
  //------------------------------------
  // method that outputs magnitude
  // response to the stream 
  // pointed-to by Response_File
  // (ordinates and normalized abscissae
  
  void DumpMagResp( void );

  double GetIntervalPeak(int beg_indx, int end_indx);
  
  
protected:

  FirFilterDesign *Filter_Design;
  int Num_Resp_Pts;
  int Db_Scale_Enabled;
  int Normalize_Enabled;
  ofstream *Response_File;
  int Num_Taps; 
  double* Mag_Resp;
  //-----------------------------------
  // stuff below is for "linear phase" filters
  //
  // filter band configuration: 1 = lowpass,  2 = highpass,
  //                            3 = bandpass, 4 = bandstop
  //int Band_Config;
  //int Fir_Type;
  //int N1, N2, N3, N4;
  
};

#endif
