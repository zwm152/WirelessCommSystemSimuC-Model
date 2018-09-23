//
//  File = sinc_interp.h
//

#ifndef _SINC_INTERP_H_
#define _SINC_INTERP_H_ 

#include "psmodel.h"
#include "signal_T.h"
#include <fstream.h>
#include "typedefs.h"


class SincInterpolator: public PracSimModel
{
public:
  
    
   SincInterpolator( char* instance_name,
                     PracSimModel* outer_model,
                     Signal<float>* in_sig);
   ~SincInterpolator(void);
   void Initialize(void);
   int Execute(void);

private:
  void AddSample(float new_samp);

  void DumpResult(ofstream* out_file);

  void CopyResult( double* output_array );

  double FindPeak( void );
    
  Signal<float> *In_Sig;
  double Digital_Samp_Intvl;
  double Analog_Samp_Intvl;
  double Delta_Arg;
  double *Analog_Signal;
  int Block_Size;
  int Curr_Samp;
  int Num_Analog_Samps;
  int Num_Signif_Sidelobes;
  int First_Dig_Samp_Idx;
  int Max_Offset;
  int Analog_Interp_Rate;
  char *Anlg_Sig_File_Name;
  ofstream *Anlg_Sig_File;
  
};

#endif
