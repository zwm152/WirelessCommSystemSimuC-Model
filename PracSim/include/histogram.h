//
//  File = histogram.h
//

#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include "psmodel.h"
#include "signal_T.h"

template <class T>
class HistogramBuilder : public PracSimModel
{
public:
   HistogramBuilder( char* instance_name,
                     PracSimModel* outer_model,
                     Signal<T>* in_sig );

   ~HistogramBuilder(void);
   void Initialize(void);
   int Execute(void);

private:
   int Block_Size;
   int Num_Segs_To_Tally;
   int Segs_In_Tally;
   Signal<T> *In_Sig;
   double Samp_Intvl;
   double *Hist_Bins;
   double Bin_Width;
   int Num_Bins;
   int Pts_In_Tally;
   int Out_Of_Range_Count;
   int Ctr_Bin;
   int Hold_Off;
   char *Hist_File_Name;
   ofstream *Hist_File;
   bool Halt_When_Completed;
   bool Processing_Completed;
   bool Positive_Only;
   double Sum;
   double Sum_Sqrs;
};

#endif //_HISTOGRAM_H_
