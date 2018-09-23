//
//  File = gensig.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "gensig.h"
#include "sigplot.h"
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern SignalPlotter SigPlot;
extern int PassNumber;

//======================================================

GenericSignal::GenericSignal( char* name, 
                              PracSimModel* model )
{
   Owning_Model = model;
   Name = new char[strlen(name)+2];
   strcpy(Name, name);
   Connected_Sigs = new std::vector<GenericSignal*>;
   Root_Id = this;
   Sig_Is_Root = true;
   Plot_Setup_Complete = false;
   Plotting_Enabled = false;
   Time_At_Beg = 0.0;
   Alloc_Mem_Depth = 0;
   Cumul_Samp_Cnt = 0;
}
//======================================================
GenericSignal::~GenericSignal( void )
{
};
//======================================================
char* GenericSignal::GetName()
{
   return(Name);
}
//======================================================
int GenericSignal::GetBlockSize()
{
   if(Sig_Is_Root)
      return(Block_Size);
   else
      return(Root_Id->GetBlockSize());
}
//======================================================
int GenericSignal::GetValidBlockSize()
{
   if(Sig_Is_Root)
      return(Valid_Block_Size);
   else
      return(Root_Id->GetValidBlockSize());
}
//======================================================
GenericSignal* GenericSignal::GetId()
{
return(Root_Id);
}
//======================================================
void GenericSignal::SetBlockSize(int block_size)
{
   if(Sig_Is_Root){
      Block_Size = block_size;
      Buf_Len = block_size;
   }
   else{
      Root_Id->SetBlockSize(block_size);
   }
   return;
}
//======================================================
void GenericSignal::SetValidBlockSize(int block_size)
{
   if(Sig_Is_Root){
      Valid_Block_Size = block_size;
   }
   else{
      Root_Id->SetValidBlockSize(block_size);
   }
   return;
}
//======================================================
double GenericSignal::GetSampIntvl()
{
   if(Sig_Is_Root)
      return(Samp_Intvl);
   else
      return(Root_Id->GetSampIntvl());
}
//======================================================
void GenericSignal::SetSampIntvl(double samp_intvl)
{
   if(Sig_Is_Root){
      Samp_Intvl = samp_intvl;
   }
   else{
      Root_Id->SetSampIntvl(samp_intvl);
   }
}
//======================================================
void GenericSignal::SetAllocMemDepth(int req_mem_depth)
{
   if(req_mem_depth > Alloc_Mem_Depth){
      if(Sig_Is_Root){
#ifdef _DEBUG
         *DebugFile << "Alloc_Mem_Depth increased from "
            << Alloc_Mem_Depth << " to "
            << req_mem_depth << " for signal "
            << Name << endl;
#endif
         Alloc_Mem_Depth = req_mem_depth;
      }
      else{
         Alloc_Mem_Depth = req_mem_depth;
         Root_Id->SetAllocMemDepth( req_mem_depth);
      }
   }
}
//======================================================
void GenericSignal::SetupPlotFile(GenericSignal* sig_id,
                                  double start_time,
                                  double stop_time,
                                  int plot_decim_rate,
                                  bool count_vice_time,
                                  bool header_desired)
{
   char *file_name;

   file_name = strcat(
      strcpy(new char[strlen(sig_id->GetName())+5],
      sig_id->GetName()),".txt\0");
   Plotter_File = new ofstream(file_name, ios::out);
   Plotting_Enabled = true;
   Plot_Start_Time = start_time;
   Plot_Stop_Time = stop_time;
   Plot_Decim_Rate = plot_decim_rate;
   Count_Vice_Time = count_vice_time;
   *DebugFile << "for signal " << Name 
              << " Plot_Decim_Rate = " 
              << Plot_Decim_Rate << endl;

   if(header_desired){
      (*Plotter_File) << "// Desired Signal: "
                      << sig_id->GetName() 
                      << endl;
   }
   delete []file_name;
   return;
}
//======================================================
void GenericSignal::SetEnclave(int enclave_num)
{
   Enclave_Num = enclave_num;
}
//======================================================
int GenericSignal::GetEnclave(void)
{
   return(Enclave_Num);
}
//======================================================
double GenericSignal::GetTimeAtBeg(void)
{
   return(Time_At_Beg);
}
//======================================================
void GenericSignal::SetTimeAtBeg(double time_at_beg)
{
   Time_At_Beg = time_at_beg;
   return;
}
