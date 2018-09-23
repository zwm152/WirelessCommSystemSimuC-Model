//
//  File = autometh.cpp
//

#include "autometh.h"

AutocorrMethCorrMtx::AutocorrMethCorrMtx( 
                                  double *signal,
                                  int seq_len,
                                  int max_lag)
{
  int j,k;
  double sum;
  double denom;
  int mode=0;

  Herm_Toep_Col_1 = new double[max_lag+1];
  Num_Rows = max_lag+1;
  Num_Cols = max_lag+1;
  denom = double(seq_len);

  for(k=0; k<=max_lag; k++)
    {
    sum = 0.0;

    for(j=0; j<(seq_len-k); j++)
      {
      sum += signal[j+k] * signal[j];
      }
      if(mode == 0)
      denom = double(seq_len);
    Herm_Toep_Col_1[k] = sum/denom;
    }
  return;
}
AutocorrMethCorrMtx::~AutocorrMethCorrMtx( )
{
  delete[] Herm_Toep_Col_1;
}
