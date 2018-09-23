//
//  File = signal_T.cpp
//

#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <complex>
#include "signal_T.h"
#include "sigplot.h"
#include "typedefs.h"
#include "complex_io.h"

#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern PracSimModel *CommSystem;
extern SignalPlotter SigPlot;
extern int EnclaveOffset[10];
extern int PassNumber;
//======================================================
//  Constructor used for creating root Signal objects

//template< class T >
//Signal<T>::Signal( char* name, PracSimModel* model )
//            :GenericSignal( name, model )
//{
//  Root_Id = this;
//  Sig_Is_Root = true;
//}
//======================================================

template< class T >
Signal<T>::Signal( Signal<T>* root_id, 
                          char* name, 
                          PracSimModel* model )
            :GenericSignal( name, model )
{
  Root_Id = root_id;
  Sig_Is_Root = false;
}
//========================================================
//  Constructor used for creating Signal objects in main
//  (owning model is hard wired to 'CommSystem' )

template< class T >
Signal<T>::Signal( char* name )
            :GenericSignal( name, CommSystem )
{
  Root_Id = this;
  Sig_Is_Root = true;
}
//===============================================
template< class T >
Signal<T>::~Signal( void )
{
  delete Buf_Beg;
};

//===============================================
template< class T >
void Signal<T>::AllocateSignalBuffer(void)
{
  *DebugFile << "now in Signal<T>::AllocateSignalBuffer" << endl;
  if(Block_Size <= 0)
    {
    *DebugFile << "attempt to allocate zero-length buffer in "
              << GetName() << endl;
    }
  Phys_Buf_Beg = NULL;
  Phys_Buf_Beg = new T[Block_Size + Alloc_Mem_Depth];
  if(Phys_Buf_Beg == NULL)
  {
      *DebugFile << "buffer allocation failed in "
              << GetName() << endl;
              exit(98);
  }

  Buf_Beg = Phys_Buf_Beg + Alloc_Mem_Depth;
  // area from Phys_Buf_Beg to Buf_Beg is block-to-block carryover
  // for devices like filters that use prior input samples in 
  // computation of current output sample
  
  Buf_Final_Mem_Beg = Phys_Buf_Beg + Block_Size;
  Cumul_Samps_Thru_Prev_Block = 0;
}
//===============================================
template< class T >
void Signal<T>::InitializeReadPtrs(void)
{
  Signal<T>* conn_sig;
  int num_sigs;
  num_sigs = int(Connected_Sigs->size());
  for(int sig_num = 0; sig_num < num_sigs; sig_num++)
    {
     conn_sig = (Signal<T>*)Connected_Sigs->at(sig_num);
     conn_sig->Buf_Beg = this->Buf_Beg;
     conn_sig->Buf_Len = this->Buf_Len;
     conn_sig->Block_Size = this->Block_Size;
     #ifdef _USE_SAMP_RATE
       conn_sig->Samp_Rate = this->Samp_Rate;
     #else
       conn_sig->Samp_Intvl = this->Samp_Intvl;
     #endif
    }
}
//===============================================
template< class T >
T* Signal<T>::GetRawOutputPtr(PracSimModel* model)
{
return(Buf_Beg);
}

//===============================================
template< class T >
T* Signal<T>::GetRawInputPtr(PracSimModel* model)
{
return(Buf_Beg);
}

//==============================================================
template< class T >
Signal<T>* Signal<T>::AddConnection(  PracSimModel* model,
                                          char* name_in_model )
{
  Signal<T>* new_sig;
  new_sig = new Signal<T>(this, name_in_model, model);
  Connected_Sigs->push_back(new_sig);
  return(new_sig);
}
//==============================================================
//template< class T >
//Signal<T>* Signal<T>::AddConnection(  PracSimModel* model,
//                                      char* name_in_model,
//                                      Signal<T>* new_sig )
//{
//  //Signal<T>* new_sig;
//  new_sig = new Signal<T>(this, name_in_model, model);
//  Connected_Sigs->push_back(new_sig);
//  return(new_sig);
//}
//===============================================
template< class T >
void Signal<T>::Dump(ofstream out_file)
{
  T *data_ptr;
//  data_ptr = Buf_Beg+32;
  data_ptr = Buf_Beg;
  for(int i=0; i<Buf_Len; i++)
    {
    out_file << i << ", " << (*data_ptr) << endl;
    data_ptr++;
    }
}
//===============================================
template< class T >
void Signal<T>::PassUpdate(void)
{
  #ifdef _USE_SAMP_RATE
    Time_At_Beg += Valid_Block_Size/Samp_Rate;
  #else
    Time_At_Beg += Valid_Block_Size * Samp_Intvl;
  #endif

  Prev_Block_Size = Valid_Block_Size;
  Cumul_Samps_Thru_Prev_Block += Valid_Block_Size;

  // copy samples for signals with memory
  if(Alloc_Mem_Depth != 0)
    {
    for(int ix=0; ix<Alloc_Mem_Depth; ix++)
      {
      Phys_Buf_Beg[ix] = Buf_Final_Mem_Beg[ix];
      }
    }
}
//===============================================
template< class T >
void Signal<T>::SetupPlotSignal(void)
{
  if(Plot_Setup_Complete) return;

  //------------------------------------------------
  // If plotting is enabled for this signal, setup the buffer
  // read pointers that will be needed to fetch the data to 
  // be plotted.

  if(Plotting_Enabled)
    {
//    Next_Loc_To_Plot = Buf_Beg;
    #ifdef _USE_SAMP_RATE
      Start_Sample = (int)(Plot_Start_Time*Samp_Rate);
      Stop_Sample = (int)((Plot_Stop_Time*Samp_Rate)
                          /Plot_Decim_Rate) * Plot_Decim_Rate;
    #else
      Start_Sample = (int)(Plot_Start_Time / Samp_Intvl);
      Stop_Sample = (int)((Plot_Stop_Time / Samp_Intvl)
                          /Plot_Decim_Rate) * Plot_Decim_Rate;
    #endif
    Plotting_Wakeup = 1 + Start_Sample/Block_Size;
    Plotting_Bedtime = 1 + (Stop_Sample/Block_Size);
    SigPlot.SetWakeAndSleep(  this,
                              Plotting_Wakeup,
                              Plotting_Bedtime);
    Plot_Setup_Complete = true;
    }
}
//============================================
#ifdef _ENCLAVES
template< class T >
void Signal<T>::IssuePlotterData(void)
{
  T *plot_data_ptr;
  int samp_cnt;
  int beg_samp_cnt;
  int end_samp_cnt;
  int next_plot_pass;
  double samp_time;

  //-----------------------------------------------------------
  // If this is the first block being plotted for this signal,
  // calculate which sample to start with.  Otherwise start
  // with the first sample in the block.

  if(PassNumber == Plotting_Wakeup)
    {
    plot_data_ptr = Buf_Beg + (Start_Sample % Buf_Len);
    beg_samp_cnt = Start_Sample % Buf_Len;
    Cumul_Samp_Cnt = Start_Sample;
    }
  else
    {
    plot_data_ptr = Next_Loc_To_Plot;
    beg_samp_cnt = (Next_Loc_To_Plot - Buf_Beg) % Block_Size;
    }
  //---------------------------------------------------------------
  // Calculate the pass in which the next samples to be plotted occur.
  // If it is after the current pass, wait (return).
  // This check is needed  to accommodate Plot_Decim_Rate greater than
  // the block size.

  next_plot_pass = Cumul_Samp_Cnt/Block_Size + 1;

  if( next_plot_pass > PassNumber ) return;

  //--------------------------------------------------------
  // If this is the final block being plotted for this signal,
  // calculate which sample to end with.  Otherwise plot
  // until the end of the block.

  if(PassNumber == Plotting_Bedtime)
    {
    end_samp_cnt = Stop_Sample % Block_Size;
    }
  else
    {
    end_samp_cnt = Block_Size;
    }
  //---------------------------------------------------------
  // Over desired time interval, get samples from signal buffer
  // and write them to the signal data file.
  for( samp_cnt = beg_samp_cnt; samp_cnt < end_samp_cnt;
       samp_cnt += Plot_Decim_Rate)
    {
    if(Count_Vice_Time)
      {
      (*Plotter_File).setf(ios::scientific | ios::right);
      (*Plotter_File) << Cumul_Samp_Cnt
                      << ",  " << *plot_data_ptr << endl;
      }
    else
      {
      //(*Plotter_File).setf(ios::scientific | ios::right);
      //(*Plotter_File) << setw(15) << setprecision(8)
      //                << Cumul_Samp_Cnt / Samp_Rate
      //                << setw(22) << setprecision(15)
      //                << "  " << *plot_data_ptr << endl;
      #ifdef _USE_SAMP_RATE
        (*Plotter_File) << Time_At_Beg + samp_cnt / Samp_Rate
                        << ",  " << *plot_data_ptr << endl;
      #else
        (*Plotter_File).flags(ios::fixed | ios::right);
        //(*Plotter_File) << setprecision(6)
        //                << Time_At_Beg + samp_cnt * Samp_Intvl
        //                << ",  " << flush;
        samp_time = Time_At_Beg - Block_Size * EnclaveOffset[Enclave_Num]*Samp_Intvl
                     + samp_cnt * Samp_Intvl;
        (*Plotter_File) << setprecision(6)
                        << samp_time
                        << ",  " << flush;
        (*Plotter_File).flags(ios::scientific | ios::right);
        (*Plotter_File) << *plot_data_ptr << endl;
      #endif
      }
    plot_data_ptr += Plot_Decim_Rate;
    Cumul_Samp_Cnt += Plot_Decim_Rate;
    }
  Next_Loc_To_Plot = plot_data_ptr;
  if(Next_Loc_To_Plot >= (Buf_Beg + Buf_Len))
    {
    Next_Loc_To_Plot = Buf_Beg + ((int)(Next_Loc_To_Plot -
                        Buf_Beg) % Block_Size);
    }
  return;
}
#endif

//#ifdef _VAR_BLOCKS
//============================================
template< class T >
void Signal<T>::IssuePlotterData(void)
{
  T *plot_data_ptr;
  int samp_cnt;
  int beg_samp_cnt;
  int end_samp_cnt;
  int next_plot_pass;
  double samp_time;

  //-----------------------------------------------------------
  // If this is the first block being plotted for this signal,
  // calculate which sample to start with.  Otherwise start
  // with the first sample in the block.

  if( Stop_Sample < Cumul_Samp_Cnt )
    {
    // plotting is completed for this signal
    Cumul_Samp_Cnt += Valid_Block_Size;
    return;
    }
  if( Start_Sample >= Cumul_Samp_Cnt + Valid_Block_Size)
    {
    // plotting does not start until a later block
    Cumul_Samp_Cnt += Valid_Block_Size;
    return;
    }

  if(Start_Sample >= Cumul_Samp_Cnt)
    {
    // plotting begins somewhere in this block
    plot_data_ptr = Buf_Beg + (Start_Sample - Cumul_Samp_Cnt);
    beg_samp_cnt = Start_Sample - Cumul_Samp_Cnt;
    }
  else
    {
    // plotting has already started
    //plot_data_ptr = Next_Loc_To_Plot;
    plot_data_ptr = Buf_Beg;
    //beg_samp_cnt = (Next_Loc_To_Plot - Buf_Beg) % Prev_Block_Size;
    beg_samp_cnt = 0;
    }

  //---------------------------------------------------------------
  // Calculate the pass in which the next samples to be plotted occur.
  // If it is after the current pass, wait (return).
  // This check is needed  to accommodate Plot_Decim_Rate greater than
  // the block size.

  next_plot_pass = Cumul_Samp_Cnt/Block_Size + 1;

  if( next_plot_pass > PassNumber ) return;

  //--------------------------------------------------------
  // If this is the final block being plotted for this signal,
  // calculate which sample to end with.  Otherwise plot
  // until the end of the block.

  if( Cumul_Samp_Cnt + Valid_Block_Size > Stop_Sample )
    {
    // plotting ends somewhere in this block
    end_samp_cnt = Stop_Sample - Cumul_Samp_Cnt;
    }
  else
    {
    // plotting continues thru end of block
    end_samp_cnt = Valid_Block_Size;
    }
  //---------------------------------------------------------
  // Over desired time interval, get samples from signal buffer
  // and write them to the signal data file.
  Cumul_Samp_Cnt += beg_samp_cnt;
  for( samp_cnt = beg_samp_cnt; samp_cnt < end_samp_cnt;
       samp_cnt += Plot_Decim_Rate)
    {
    if(Count_Vice_Time)
      {
      (*Plotter_File).setf(ios::scientific | ios::right);
      (*Plotter_File) << Cumul_Samp_Cnt
                      << ",  " << *plot_data_ptr << endl;
      }
    else
      {
      //(*Plotter_File).setf(ios::scientific | ios::right);
      //(*Plotter_File) << setw(15) << setprecision(8)
      //                << Cumul_Samp_Cnt / Samp_Rate
      //                << setw(22) << setprecision(15)
      //                << "  " << *plot_data_ptr << endl;
      #ifdef _USE_SAMP_RATE
        (*Plotter_File) << Time_At_Beg + samp_cnt / Samp_Rate
                        << ",  " << *plot_data_ptr << endl;
      #else
        (*Plotter_File).flags(ios::fixed | ios::right);
        //(*Plotter_File) << setprecision(6)
        //                << Time_At_Beg + samp_cnt * Samp_Intvl
        //                << ",  " << flush;
//        samp_time = Time_At_Beg - Block_Size * EnclaveOffset[Enclave_Num]*Samp_Intvl
//                     + samp_cnt * Samp_Intvl;
        samp_time = Time_At_Beg + samp_cnt * Samp_Intvl;
        (*Plotter_File) << setprecision(6)
                        << samp_time
                        << ",  " << flush;
        (*Plotter_File).flags(ios::scientific | ios::right);
        (*Plotter_File) << *plot_data_ptr << endl;
      #endif
      }
    plot_data_ptr += Plot_Decim_Rate;
    if(plot_data_ptr >= (Buf_Beg + Buf_Len))
    {
    int debug_flag=1;
    }
    Cumul_Samp_Cnt += Plot_Decim_Rate;
    }
  Next_Loc_To_Plot = plot_data_ptr;
  if(Next_Loc_To_Plot >= (Buf_Beg + Buf_Len))
    {
    Next_Loc_To_Plot = Buf_Beg + ((int)(Next_Loc_To_Plot -
                        Buf_Beg) % Block_Size);
    }
  return;
}
//#endif //_VAR_BLOCKS
template Signal<float>;
//template Signal<bit_t>;
template Signal<byte_t>;
template Signal< std::complex<float> >;
template Signal< int >;


