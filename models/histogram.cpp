//
//  File =histogram.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "model_graph.h"
#include "histogram.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

template <class T>
HistogramBuilder<T>
         ::HistogramBuilder( char* instance_name,
                             PracSimModel* outer_model,
                             Signal<T>* in_sig )
                :PracSimModel( instance_name,
                                outer_model )
{
   MODEL_NAME(SpectrumAnalyzer_T);
   OPEN_PARM_BLOCK;
   GET_INT_PARM(Num_Segs_To_Tally);
   GET_INT_PARM(Hold_Off);

   Hist_File_Name = new char[64];
   strcpy(Hist_File_Name, "\0");
   GET_STRING_PARM(Hist_File_Name);
   GET_INT_PARM(Num_Bins);
   GET_DOUBLE_PARM(Bin_Width);
   GET_BOOL_PARM(Positive_Only);
   GET_BOOL_PARM(Halt_When_Completed);

   if(!Positive_Only){
      if(Num_Bins%2 == 0) Num_Bins++;
      Ctr_Bin = (Num_Bins-1)/2;
   }
   else{
      Ctr_Bin = 0;
   }

   In_Sig = in_sig;
   MAKE_INPUT(In_Sig);


   Hist_Bins = new double[Num_Bins];
   for(int is=0; is<Num_Bins; is++){
      Hist_Bins[is] = 0.0;
   }

   Hist_File = new ofstream(Hist_File_Name, ios::out);
   Processing_Completed = false;

}
template <class T>
HistogramBuilder<T>::~HistogramBuilder( void ){ };

template <class T>
void HistogramBuilder<T>::Initialize(void)
{
   Segs_In_Tally = 0;
   Pts_In_Tally = 0;
   Out_Of_Range_Count = 0;
   Block_Size = In_Sig->GetBlockSize();
   Sum = 0.0;
   Sum_Sqrs = 0.0;

};

template <class T>
//======================================================
int HistogramBuilder<T>::Execute()
{
   int is;
   float left_edge, right_edge, gap_on_left;
#ifdef _DEBUG
   *DebugFile << "In HistogramBuilder::Execute\0" 
              << endl;
#endif


   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off) return (_MES_AOK);
   //--------------------------------
   //  Get pointers for buffers

   T *in_sig_ptr = GET_INPUT_PTR(In_Sig);
   T in_sig_val;
   int bin_idx;
   double val;
   double edge_offset;
   double mean, variance;

   for(is=0; is<Block_Size; is++){
      in_sig_val = *in_sig_ptr++;
      Sum += in_sig_val;
      Sum_Sqrs += in_sig_val * in_sig_val;
      bin_idx = int(Ctr_Bin + in_sig_val/Bin_Width);
      if(bin_idx>=0 && bin_idx<Num_Bins){
         Hist_Bins[bin_idx]++;
         Pts_In_Tally++;
      }
      else{
         Out_Of_Range_Count++;
      }
   }

   Segs_In_Tally++;

   // is it time to dump the results?
   if(Segs_In_Tally == Num_Segs_To_Tally){
      mean = Sum/Pts_In_Tally;
      variance = (Sum_Sqrs/Pts_In_Tally) - (mean*mean);
      *DebugFile << "mean = " << mean << endl;
      *DebugFile << "variance = " << variance << endl;

      if(Positive_Only)
         edge_offset = 0.0;
      else
         edge_offset = 0.5;

      for(is=0; is<Num_Bins; is++){
         gap_on_left = 
            float(((is-Ctr_Bin)-edge_offset)*Bin_Width);
         left_edge = 
            float(((is-Ctr_Bin)-edge_offset)*Bin_Width);
         right_edge = float(left_edge + Bin_Width);
         val = 
            double(Hist_Bins[is])/double(Pts_In_Tally);
         (*Hist_File) << gap_on_left << ", " << 0 
                      << endl;
         (*Hist_File) << left_edge << ", " << val 
                      << endl;
         (*Hist_File) << right_edge << ", " << val 
                      << endl;
      }
      gap_on_left = 
         float(((Num_Bins-Ctr_Bin)-0.5)*Bin_Width);
      (*Hist_File) << gap_on_left << ", " << 0 << endl;

      Processing_Completed = true;
      Hist_File->close();
      if(Halt_When_Completed){
#ifdef _DEBUG
         *DebugFile << "Execution halted by " 
                    << GetModelName() << endl;
#endif
         exit(0);
      }
   }
   return(_MES_AOK);
}
//======================================================
//template HistogramBuilder<std::complex<float> >;
template HistogramBuilder<float>;
