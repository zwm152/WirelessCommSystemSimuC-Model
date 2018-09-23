//
//  File = cmpx_iq_plot.h
//

#ifndef _CMPX_IQ_PLOT_H_
#define _CMPX_IQ_PLOT_H_

#include "signal_T.h"
#include "psmodel.h"

class CmpxIqPlot : public PracSimModel
{
public:
   CmpxIqPlot(  
         char* instance_nam,
         PracSimModel* outer_model,
         Signal< std::complex<float> >* in_sig,
         Signal< bit_t >* symb_clock_in );

   ~CmpxIqPlot(void);
   void Initialize(void);
   int Execute(void);

private:
   int Block_Size;
   Signal< std::complex<float> > *In_Sig;  
   Signal< bit_t > *Symb_Clock_In;  
   int Hold_Off;
   int Sample_Strobe;
   int Sampling_Offset;
   bool Processing_Completed;
   char *Plot_File_Name;
   ofstream *Plot_File;
};

#endif
