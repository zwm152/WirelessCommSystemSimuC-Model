//
//  File =ogive.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "model_graph.h"
#include "ogive.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif

extern ParmFile *ParmInput;
extern int PassNumber;

//======================================================

template <class T>
OgiveBuilder<T>::OgiveBuilder( char* instance_name,
                                  PracSimModel* outer_model,
                                  Signal<T>* in_sig )
                :PracSimModel( instance_name,
                                outer_model )
{
  MODEL_NAME(OgiveBuilder);
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

  if(!Positive_Only)
  {
     if(Num_Bins%2 == 0) Num_Bins++;
     First_Pos_Bin = (Num_Bins-1)/2;
  }
  else
  {
      First_Pos_Bin = 0;
   }

  In_Sig = in_sig;
  MAKE_INPUT(In_Sig);


  Hist_Bins = new int[Num_Bins];
  for(int is=0; is<Num_Bins; is++)
    {
    Hist_Bins[is] = 0;
    }

  Hist_File = new ofstream(Hist_File_Name, ios::out);
  Processing_Completed = false;

}
template <class T>
OgiveBuilder<T>::~OgiveBuilder( void ){ };

template <class T>
void OgiveBuilder<T>::Initialize(void)
{
   Segs_In_Tally = 0;
   Pts_In_Tally = 0;
   Block_Size = In_Sig->GetBlockSize();
   //Samp_Intvl = In_Sig->GetSampIntvl();

};

template <class T>
int OgiveBuilder<T>::Execute()
{
   int is;
   float left_edge, right_edge, gap_on_left;
   #ifdef _DEBUG
      *DebugFile << "In OgiveBuilder::Execute\0" << endl;
   #endif


   if(Processing_Completed) return(_MES_AOK);
   if(PassNumber < Hold_Off) return (_MES_AOK);
   //--------------------------------
   //  Get pointers for buffers

   T *in_sig_ptr = GET_INPUT_PTR(In_Sig);
   T in_sig_val;
   int bin_idx;
   int sum;
   double val;

   for(is=0; is<Block_Size; is++)
   {
      in_sig_val = *in_sig_ptr++;
      bin_idx = First_Pos_Bin + int(floor(in_sig_val/Bin_Width));
      Hist_Bins[bin_idx]++;
      Pts_In_Tally++;
   }

   Segs_In_Tally++;

   // is it time to dump the results?
   if(Segs_In_Tally == Num_Segs_To_Tally)
   {
      sum = 0;
      for(is=0; is<Num_Bins; is++)
      {
         gap_on_left = float((is-First_Pos_Bin)*Bin_Width);
         left_edge = float((is-First_Pos_Bin)*Bin_Width);
         right_edge = float(((is-First_Pos_Bin)+1.0)*Bin_Width);
         sum += Hist_Bins[is];
         val = double(sum)/double(Pts_In_Tally);
         (*Hist_File) << gap_on_left << ", " << 0 << endl;
         (*Hist_File) << left_edge << ", " << val << endl;
         (*Hist_File) << right_edge << ", " << val << endl;
      }
      gap_on_left = float((is-First_Pos_Bin)*Bin_Width);
      (*Hist_File) << gap_on_left << ", " << 0 << endl;

      Processing_Completed = true;
      Hist_File->close();
      if(Halt_When_Completed)
      {
         #ifdef _DEBUG
            *DebugFile << "Execution halted by " << GetModelName() << endl;
         #endif
         exit(0);
      }
   }

   return(_MES_AOK);

}
//template HistogramBuilder<std::complex<float> >;
template OgiveBuilder<float>;
