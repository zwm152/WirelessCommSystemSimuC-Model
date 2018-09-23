//
//  File = gensig.h
//

#ifndef _GENSIG_H_
#define _GENSIG_H_

#include "vector"
#include "sigstuff.h"
#include <iostream>
#include <fstream>
using namespace std;
//#include "psmodel.h"
class PracSimModel;
//======================================================
class GenericSignal
{
public:
   GenericSignal( char* name, PracSimModel* model );
   ~GenericSignal(void);
   int GetBlockSize();
   int GetValidBlockSize();
   void SetBlockSize(int block_size);
   void SetValidBlockSize(int block_size);
   char* GetName(void);
   double GetSampIntvl(void);
   void SetSampIntvl(double samp_intvl);
   void SetAllocMemDepth( int req_mem_depth );
   virtual void AllocateSignalBuffer(void){};
   GenericSignal* GetId();
   virtual void InitializeReadPtrs(void){};
   virtual void SetupPlotSignal(void){};
   virtual void IssuePlotterData(void){};
   void SetupPlotFile( GenericSignal* sig_id,
                       double start_time,
                       double stop_time,
                       int plot_decim_rate,
                       bool count_vice_time,
                       bool header_desired);
   virtual void PassUpdate(void){};
   double GetTimeAtBeg(void);
   void SetTimeAtBeg(double time_at_beg);
   void SetEnclave(int enclave_num);
   int GetEnclave(void);
protected:
   int Buf_Len;
   int Block_Size;
   int Valid_Block_Size;
   int Prev_Block_Size;
   long Cumul_Samps_Thru_Prev_Block;
   int Alloc_Mem_Depth;
   double Samp_Intvl;
   char* Name;
   PracSimModel* Owning_Model;
   GenericSignal* Root_Id;
   bool Sig_Is_Root;
   double Plot_Start_Time;
   double Plot_Stop_Time;
   int Plot_Decim_Rate;
   ofstream* Plotter_File;
   bool Plotting_Enabled;
   bool Plot_Setup_Complete;
   bool Count_Vice_Time;
   int Start_Sample;
   int Stop_Sample;
   int Plotting_Wakeup;
   int Plotting_Bedtime;
   int Cumul_Samp_Cnt;
   double Time_At_Beg;
   int Enclave_Num;
   std::vector<GenericSignal*> *Connected_Sigs;
};

#endif //_GENSIG_H_
