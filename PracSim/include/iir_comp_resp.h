//
//  File = iir_comp_resp.h
//

#ifndef _IIR_COMP_RESP_H_
#define _IIR_COMP_RESP_H_

#include <iostream> 
#include <fstream>
using namespace std;

class IirComputedResponse
{
public:

  IirComputedResponse(  double *numer_coeff,
                        double *denom_coeff,
                        int filt_order,
                        double samp_intvl,
                        int num_resp_pts,
                        bool db_scale_enabled );  
  void NormalizeResponse( void );                        
  double* GetMagResp( void);   
  void DumpMagResp( ofstream *resp_file );  
  
protected:
  int Num_Resp_Pts;
  bool Db_Scale_Enabled;
  double Samp_Intvl;
  std::complex<double> *Freq_Resp;
  double* Mag_Resp;
  double *Phase_Resp;
  
};

#endif
