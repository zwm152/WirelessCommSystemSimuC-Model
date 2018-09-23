//
//  File = polar_freq_dom_filt.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "polar_freq_dom_filt.h"
#include "misdefs.h"
#include "model_graph.h"
#include "sigplot.h"
#include "dit_pino_T.h"
#include "dit_nipo_T.h"
#include "complex_io.h"
extern ParmFile *ParmInput;
extern SignalPlotter SigPlot;
extern int PassNumber;
//ofstream CorrFile("corr_res.txt", ios::out);

//======================================================

PolarFreqDomainFilter::PolarFreqDomainFilter( char* instance_name,
                                         PracSimModel* outer_model,
                                         Signal< std::complex<float> >* in_sig,
                                         Signal< std::complex<float> >* out_sig)
            :PracSimModel(instance_name,
                          outer_model)
{
   MODEL_NAME(PolarFreqDomainFilter);
   ENABLE_MULTIRATE;
   In_Sig = in_sig;
   Out_Sig = out_sig;

   OPEN_PARM_BLOCK;

   GET_INT_PARM(Fft_Size);
   GET_DOUBLE_PARM(Dt_For_Fft);
   GET_FLOAT_PARM(Overlap_Save_Mem);
   GET_BOOL_PARM(Bypass_Enabled);

   Magnitude_Data_Fname = new char[64];
   strcpy(Magnitude_Data_Fname, "\0");
   GET_STRING_PARM(Magnitude_Data_Fname);
   GET_DOUBLE_PARM(Mag_Freq_Scaling_Factor);

   Phase_Data_Fname = new char[64];
   strcpy(Phase_Data_Fname, "\0");
   GET_STRING_PARM(Phase_Data_Fname);
   GET_DOUBLE_PARM(Phase_Freq_Scaling_Factor);

   Num_Saved_Samps = int(Overlap_Save_Mem/Dt_For_Fft + 0.5);
   Block_Size = Fft_Size - Num_Saved_Samps;

   MAKE_OUTPUT(Out_Sig);
   MAKE_INPUT(In_Sig);

   SET_SAMP_INTVL(In_Sig, Dt_For_Fft);
   SET_BLOCK_SIZE(In_Sig, Block_Size);

   SET_SAMP_INTVL(Out_Sig, Dt_For_Fft);
   SET_BLOCK_SIZE(Out_Sig, Block_Size);

   //SET_DELAY( In_Sig, Out_Sig, Group_Delay_Offset);

}
PolarFreqDomainFilter::~PolarFreqDomainFilter( void )
{
   if(Full_Buffer != NULL) delete []Full_Buffer;
   delete []Magnitude_Data_Fname;
   delete []Resid_Data_Fname;
   delete []Stretched_Data_Fname;
};

void PolarFreqDomainFilter::Initialize(void)
{
   int i;
   int samp_num, bin_num;
   char line_buf[100];
   char *item;
   double tmp_nsexp, frac_part, int_part;
   double min_data_freq, max_data_freq;
   double left_freq, right_freq;
   double bin_freq, left_val, right_val, slope, base;
   std::complex<float> *time_resp;
   //PointDataFile input_file;
   std::complex<float> exponent;
   std::complex<float> pseudo_complex;
   ofstream *resp_file;

   tmp_nsexp = log10(double(Fft_Size))/log10(2.0);
   frac_part = modf(tmp_nsexp, &int_part);

   double delta_f = 1.0/(Fft_Size*Dt_For_Fft);

   //------------------------------------------------------------
   //  initialize derived parameters

   Ns_Exp = int_part;

   Full_Buffer = new std::complex<float>[Fft_Size];
   for(i=0; i<Fft_Size; i++)
   {
      Full_Buffer[i] = std::complex<float>(0.0,0.0);
   }

   time_resp = new std::complex<float>[Fft_Size];
   for(i=0; i<Fft_Size; i++)
   {
      time_resp[i] = std::complex<float>(0.0,0.0);
   }
   FftDitNipo( time_resp, Fft_Size);

   Mag_Resp = new float[Fft_Size];
   Phase_Resp = new float[Fft_Size];

   //-----------------------------------------------------
   //  Read in the raw response data

   Magnitude_Data_File = new ifstream(Magnitude_Data_Fname, ios::in);
   *Magnitude_Data_File >> Num_Mag_Samps;
   Magnitude_Data_File->getline(line_buf,100);
   Raw_Magnitude_Resp = new float[Num_Mag_Samps];
   Freqs_For_Magnitude = new float[Num_Mag_Samps];
   for(samp_num=0;samp_num<Num_Mag_Samps; samp_num++)
   {
      Magnitude_Data_File->getline(line_buf,100);
      item = strtok(line_buf,",\n\t");
      Freqs_For_Magnitude[samp_num] = atof(item);
      item = strtok(NULL,",\n\t");
      Raw_Magnitude_Resp[samp_num] = atof(item);
   }
   Magnitude_Data_File->close();

   resp_file = new ofstream("mag_resp.txt\0", ios::out);

   min_data_freq = Mag_Freq_Scaling_Factor * Freqs_For_Magnitude[0];
   max_data_freq = Mag_Freq_Scaling_Factor * Freqs_For_Magnitude[Num_Mag_Samps-1];

   samp_num=-1;
   right_freq = Mag_Freq_Scaling_Factor * Freqs_For_Magnitude[0];
   right_val = pow(10.0,(Raw_Magnitude_Resp[0]/20.0));
   left_freq = -delta_f*Fft_Size/2;
   slope = right_val/(right_freq-left_freq);
   base = -left_freq*slope;

   for(bin_num=-Fft_Size/2;bin_num<0;bin_num++)
   {
      bin_freq = bin_num * delta_f;


      if(bin_freq < min_data_freq)
      //
      // put straight-line skirt on negative frequency portion
      // not covered by input data
      {
         Mag_Resp[Fft_Size+bin_num] = bin_freq * slope + base;
      }
      else
      //
      //  do negative-frequency portion that is covered by input data
      {
         if(bin_freq >= right_freq) 
         {
            samp_num++;
            left_freq = right_freq;
            right_freq = Mag_Freq_Scaling_Factor * Freqs_For_Magnitude[samp_num+1];
            left_val = pow(10.0, Raw_Magnitude_Resp[samp_num]/20.0);
            right_val = pow(10.0, Raw_Magnitude_Resp[samp_num+1]/20.0);
            slope = (right_val - left_val)/(right_freq - left_freq);
            base = left_val - left_freq*slope;
         }
         Mag_Resp[Fft_Size+bin_num] = bin_freq * slope + base;
      }
      *resp_file << bin_num << ", " << Mag_Resp[Fft_Size+bin_num] << endl;
   }

   //
   //  do the positive frequency portion which is covered by input data
   bin_freq = 0;
   bin_num = 0;
   while(bin_freq<max_data_freq && bin_num<Fft_Size/2)
   {
      bin_freq = bin_num * delta_f;
      if(bin_freq >= right_freq)
      {
         samp_num++;
         left_freq = right_freq;
         right_freq = Mag_Freq_Scaling_Factor * Freqs_For_Magnitude[samp_num+1];
         left_val = pow(10.0, Raw_Magnitude_Resp[samp_num]/20.0);
         right_val = pow(10.0, Raw_Magnitude_Resp[samp_num+1]/20.0);
         slope = (right_val - left_val)/(right_freq - left_freq);
         base = left_val - left_freq*slope;
      }
      Mag_Resp[bin_num] = bin_freq * slope + base;
      *resp_file << bin_num << ", " << Mag_Resp[bin_num] << endl;
      bin_num++;
   }
   //
   // put straight-line skirt on positive frequency portion not coverd by input data
   left_freq = Mag_Freq_Scaling_Factor * Freqs_For_Magnitude[Num_Mag_Samps-1];
   left_val = pow(10.0,(Raw_Magnitude_Resp[Num_Mag_Samps-1]/20.0));
   right_freq = delta_f*(Fft_Size-2)/2;
   slope = -left_val/(right_freq-left_freq);
   base = left_val-left_freq*slope;
   while(bin_num<Fft_Size/2)
   {
      bin_freq = bin_num * delta_f;
      Mag_Resp[bin_num] = bin_freq * slope + base;
      *resp_file << bin_num << ", " << Mag_Resp[bin_num] << endl;
      bin_num++;
   }
   resp_file->close();

   Phase_Data_File = new ifstream(Phase_Data_Fname, ios::in);
   *Phase_Data_File >> Num_Phase_Samps;
   Phase_Data_File->getline(line_buf,100);

   Raw_Phase_Resp = new float[Num_Phase_Samps];
   Freqs_For_Phase = new float[Num_Phase_Samps];

   for(samp_num=0;samp_num<Num_Phase_Samps; samp_num++)
   {
      Phase_Data_File->getline(line_buf,100);
      item = strtok(line_buf,",\n\t");
      Freqs_For_Phase[samp_num] = atof(item);
      item = strtok(NULL,",\n\t");
      Raw_Phase_Resp[samp_num] = atof(item);
   }
   Phase_Data_File->close();

   resp_file = new ofstream("Phase_resp.txt\0", ios::out);

   min_data_freq = Mag_Freq_Scaling_Factor * Freqs_For_Phase[0];
   max_data_freq = Mag_Freq_Scaling_Factor * Freqs_For_Phase[Num_Phase_Samps-1];

   samp_num=-1;
   right_freq = Phase_Freq_Scaling_Factor * Freqs_For_Phase[0];
   right_val = Raw_Phase_Resp[0];
   left_freq = -delta_f*Fft_Size/2;
   slope = right_val/(right_freq-left_freq);
   base = -left_freq*slope;

   for(bin_num=-Fft_Size/2;bin_num<0;bin_num++)
   {
      bin_freq = bin_num * delta_f;
      if(bin_freq < min_data_freq)
      //
      // put straight-line skirt on negative frequency portion
      // not covered by input data
      {
         //Phase_Resp[Fft_Size+bin_num] = bin_freq * slope + base;
         Phase_Resp[Fft_Size+bin_num] = Raw_Phase_Resp[0];
      }
      else
      //
      //  do negative-frequency portion that is covered by input data
      {
         if(bin_freq >= right_freq) 
         {
            samp_num++;
            left_freq = right_freq;
            right_freq = Phase_Freq_Scaling_Factor * Freqs_For_Phase[samp_num+1];
            left_val = Raw_Phase_Resp[samp_num];
            right_val = Raw_Phase_Resp[samp_num+1];
            slope = (right_val - left_val)/(right_freq - left_freq);
            base = left_val - left_freq*slope;
         }
         Phase_Resp[Fft_Size+bin_num] = bin_freq * slope + base;
      }
      *resp_file << bin_num << ", " << Phase_Resp[Fft_Size+bin_num] << endl;
   }

   //
   //  do the positive frequency portion which is covered by input data
   bin_freq = 0;
   bin_num = 0;
   while(bin_freq<max_data_freq && bin_num<Fft_Size/2)
   {
      bin_freq = bin_num * delta_f;
      if(bin_freq >= right_freq)
      {
         samp_num++;
         left_freq = right_freq;
         right_freq = Phase_Freq_Scaling_Factor * Freqs_For_Phase[samp_num+1];
         left_val = Raw_Phase_Resp[samp_num];
         right_val = Raw_Phase_Resp[samp_num+1];
         slope = (right_val - left_val)/(right_freq - left_freq);
         base = left_val - left_freq*slope;
      }
      Phase_Resp[bin_num] = bin_freq * slope + base;
      *resp_file << bin_num << ", " << Phase_Resp[bin_num] << endl;
      bin_num++;
   }
   //
   // put straight-line skirt on positive frequency portion not coverd by input data
   left_freq = Phase_Freq_Scaling_Factor * Freqs_For_Phase[Num_Phase_Samps-1];
   left_val = Raw_Phase_Resp[Num_Phase_Samps-1];
   right_freq = delta_f*(Fft_Size-2)/2;
   slope = -left_val/(right_freq-left_freq);
   base = left_val-left_freq*slope;
   while(bin_num<Fft_Size/2)
   {
      bin_freq = bin_num * delta_f;
      //Phase_Resp[bin_num] = bin_freq * slope + base;
      Phase_Resp[bin_num] = Raw_Phase_Resp[Num_Phase_Samps-1];
      *resp_file << bin_num << ", " << Phase_Resp[bin_num] << endl;
      bin_num++;
   }
   resp_file->close();
   //-----------------------------------------------------

}
//#define RAD_PER_DEG 0.017453293
int PolarFreqDomainFilter::Execute()
{
   int is, ns_exp;
   int block_size, num_saved_samps;
   int valid_block_size;
   std::complex<float> *in_sig_ptr;
   std::complex<float> *out_sig_ptr;
   float phase, magnitude;

   //-------------------------------------------------------
   //  Copy frequently accessed member vars into local vars
   block_size = Block_Size;
   num_saved_samps = Num_Saved_Samps;
   ns_exp = Ns_Exp;

   //----------------------------------------
   // Get pointers for input and output
   in_sig_ptr = GET_INPUT_PTR(In_Sig);
   out_sig_ptr = GET_OUTPUT_PTR(Out_Sig);
   valid_block_size = In_Sig->GetValidBlockSize();
   Out_Sig->SetValidBlockSize(valid_block_size);

   //--------------------------------------

   memcpy(Full_Buffer, in_sig_ptr, Block_Size*sizeof(std::complex<float>));

   if(PassNumber == 2)
   {
      ofstream *pass_in_file = new ofstream("FftInput_2.txt\0",ios::out);
      int ii;
      for(ii=Block_Size; ii<Fft_Size; ii++)
      {
         //*pass_in_file << ((PassNumber-2)*Fft_Size+ii) << ", " << Full_Buffer[ii] << endl;
         *pass_in_file << (ii - Fft_Size) << ", " << Full_Buffer[ii] << endl;
      }
      for(ii=0; ii<Fft_Size; ii++)
      {
         //*pass_in_file << ((PassNumber-1)*Fft_Size+ii) << ", " << Full_Buffer[ii] << endl;
         *pass_in_file << ii << ", " << Full_Buffer[ii] << endl;
      }
      pass_in_file->close();
      //exit(77);
   }

   // transform block of input samples
   FftDitNipo( Full_Buffer, Fft_Size);

   if(!Bypass_Enabled)
   {
      // multiply by sampled frequency response
      for( is=0; is<Fft_Size; is++)
      {
         phase = std::arg(Full_Buffer[is]) + RAD_PER_DEG*Phase_Resp[is];
         magnitude = std::abs(Full_Buffer[is]) * Mag_Resp[is];
         Full_Buffer[is]=std::complex<float>(magnitude*cos(phase), magnitude*sin(phase));
      }
   }

   // transform back to time domain
   IfftDitNipo( Full_Buffer, Fft_Size);

   // copy results to output buffer
   memcpy(out_sig_ptr, Full_Buffer, Block_Size*sizeof(std::complex<float>));
   for(int ix=0;ix<Block_Size;ix++)
   {
      out_sig_ptr[ix] /= Fft_Size;
   }

//   if(PassNumber == 2)
//   {
//      ofstream *sing_file = new ofstream("FftOutput_2.txt\0",ios::out);
//      for(int ii=0; ii<Fft_Size; ii++)
//      {
//         //*sing_file << ((PassNumber-1)*46+ii) << ", " << Full_Buffer[ii] << endl;
//         *sing_file << ii << ", " << Full_Buffer[ii] << endl;
//      }
//      sing_file->close();
//      exit(77);
//   }

   if(Block_Size >= Num_Saved_Samps)
   {
      memcpy(  &Full_Buffer[Block_Size],
               &in_sig_ptr[block_size-num_saved_samps],
               num_saved_samps*sizeof(std::complex<float>));
   }
   else
   {
      memcpy(  &Full_Buffer[Block_Size],
               &in_sig_ptr[2*block_size],
               (num_saved_samps-block_size)*sizeof(std::complex<float>));

      memcpy(  &Full_Buffer[num_saved_samps],
               in_sig_ptr,
               block_size*sizeof(std::complex<float>));
   }
   //---------------------
   return(_MES_AOK);
}

