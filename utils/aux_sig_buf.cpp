//
//  File = aux_sig_buf.cpp
//

#include <stdlib.h>
#include <fstream>
//#include <math.h>
//#include "parmfile.h"
#include "aux_sig_buf.h"
//#include "misdefs.h"
//extern ParmFile *ParmInput;
#include "psstream.h"
   
extern PracSimStream ErrorStream;
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
//extern int PassNumber;
//ofstream PowerDebug("power.txt", ios::out);

//======================================================
// constructor - parms read from ParmFile

template <class T>
AuxSignalBuffer<T>::AuxSignalBuffer( T sample, int nominal_block_size )
{
   Max_Sample_Count = 3*nominal_block_size;
   Buffer_Start = new T[Max_Sample_Count];
   Write_Ptr = Buffer_Start;
   Sample_Count = 0;
}
//=============================================
template <class T>
AuxSignalBuffer<T>::~AuxSignalBuffer( void )
{ 
   delete[] Buffer_Start;
};

//===========================================
template <class T>
T* AuxSignalBuffer<T>::Load(  T *in_sig_ptr,
                              int in_sig_block_size,
                              int *in_sig_buf_count)
{
   int idx;
   T *input_ptr = in_sig_ptr;

   Sample_Count += in_sig_block_size;
   if(Sample_Count > Max_Sample_Count)
   {
      ErrorStream << "Too many samples in AuxSignalBuffer" << endl;
      exit(-99);
   }
   for(idx=0; idx<in_sig_block_size; idx++)
   {
      *Write_Ptr++ = *input_ptr++;
   }
   *in_sig_buf_count = Sample_Count;
   return(Buffer_Start);
}
//===========================================
template <class T>
void AuxSignalBuffer<T>::Release(  int num_samps_to_release)
{
   T* src_ptr;
   T* dest_ptr;
   int idx;

   Sample_Count -= num_samps_to_release;
   Write_Ptr -= num_samps_to_release;
   src_ptr = Buffer_Start + num_samps_to_release;
   dest_ptr = Buffer_Start;

   for(idx=0; idx<Sample_Count; idx++)
   {
      *dest_ptr++ = *src_ptr++;
   }
}
template AuxSignalBuffer<float>;

