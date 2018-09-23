//
//  File = dump_spect.cpp
//

#include <stdlib.h>
#include "dump_spect.h"
#include "math.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

void DumpSpectrum(  double *psd_est,
               int nsamps_in_psd,
               double delta_f,
               double freq_norm_factor,
               bool output_in_decibels,
               bool plot_two_sided,
               ofstream *psd_file)
{
   int is;
   //------------------------------------
   // find peak

   double peak_val=0.0;
   int peak_idx=0;
   for(is=0; is<nsamps_in_psd/2; is++)
   {
      if(psd_est[is] <= peak_val) continue;
      //else
      peak_val = psd_est[is];
      peak_idx = is;
   }

   if(output_in_decibels)
   {
      //double offset = 10.0*log10(Psd_Est[peak_idx]);
      double offset = 0.0;

      if(plot_two_sided)
      {
         for(is=-(nsamps_in_psd/2-1); is<0; is++)
         {
            if( psd_est[-is] >0 )
            {
               (*psd_file) << is * delta_f * freq_norm_factor << ", " 
                           << (10.0*log10(psd_est[-is])) << endl;
                          // << (10.0*log10(Psd_Est[-is])-offset) << endl;
            }
            else
            {
               (*psd_file) << is * delta_f * freq_norm_factor << ", -200.0" << endl;
            }
         }
      }

      for(is=0; is<nsamps_in_psd/2; is++)
      {
         if( psd_est[is] >0 )
         {
            (*psd_file) << is * delta_f * freq_norm_factor << ", " 
                        << (10.0*log10(psd_est[is])) << endl;
                       // << (10.0*log10(Psd_Est[is])-offset) << endl;
            psd_est[is] = 0.0;
         }
         else
         {
            (*psd_file) << is * delta_f * freq_norm_factor << ", -200.0" << endl;
         }
      }
   }
   else
   {
      // plot as linear ordinate
      if(plot_two_sided)
      {
         for(is=-(nsamps_in_psd/2-1); is<0; is++)
         {
            (*psd_file) << is * delta_f * freq_norm_factor << ", " 
                        << psd_est[-is] << endl;
         }
      }

      for(is=0; is<nsamps_in_psd/2; is++)
      {
         (*psd_file) << is * delta_f * freq_norm_factor << ", " 
                     << psd_est[is] << endl;
         psd_est[is] = 0.0;
      }
   }
}
