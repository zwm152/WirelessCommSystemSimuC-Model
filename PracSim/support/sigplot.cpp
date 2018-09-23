//
//  File = sigplot.cpp
//

#include <stdlib.h>
#include <fstream>
#include "parmfile.h"
#include "sigplot.h"
#include "syst_graph.h"
#ifdef _DEBUG
  extern ofstream *DebugFile;
#endif
extern PracSimModel *CommSystem;
extern ParmFile *ParmInput;
extern int PassNumber;
extern int EnclaveNumber;
extern SystemGraph CommSystemGraph;


//===============================================
// constructor
SignalPlotter::SignalPlotter(void)
{
  Plot_Sig_Id = new std::vector<GenericSignal*>;
};
//===============================================
// destructor
SignalPlotter::~SignalPlotter(void)
{
}
//===============================================
void SignalPlotter::Initialize(void)
{
//  int signal_num;
  int num_plot_sigs;
  char* instance_name;
  char* sig_name;
  double start_time, stop_time;
  int plot_decim_rate;
  bool file_header_req;
  bool count_vice_time;
  GenericSignal *sig_id;
  num_plot_sigs = 0;

  //Model_Name = COPY("SignalPlotter");
  instance_name = new char[strlen("SignalPlotter\0")];
  strcpy(instance_name, "SignalPlotter\0");


  OPEN_PARM_BLOCK;
  //GET_INT_PARM( Num_Plot_Sigs );
  //avoid macro so parm will not be in reports
  Num_Plot_Sigs = ParmInput->GetIntParm("Num_Plot_Sigs\0");
  int num_reject_sigs = 0;

  Wakeup_Pass = new int[Num_Plot_Sigs];
  Bedtime_Pass = new int[Num_Plot_Sigs];

  sig_name = new char[60];
  int sig_num = -1;
  if(Num_Plot_Sigs > 0)
    {
    // read signal name, start time, stop time
    for(int sig_idx = 0; sig_idx<Num_Plot_Sigs; sig_idx++)
      {
      ParmInput->GetPlotSpec(  sig_name, 
                              &start_time, 
                              &stop_time,
                              &plot_decim_rate,
                              &count_vice_time,
                              &file_header_req);
      cout << "sig_name = " << sig_name << endl;
      cout << "start_time = " << start_time << endl;
      cout << "stop_time = " << stop_time << endl;
      //cin >> sig_name;
      //cin >> start_time;
      //cin >> stop_time;
      sig_id = CommSystemGraph.GetSignalId( sig_name );
      if(sig_id != NULL)
        {
        sig_num++;
        Plot_Sig_Id->push_back(sig_id);

        //------------------------------------------
        // check to make sure same signal is not in
        // plotting list more than once
        int sig_idx;
        for( sig_idx = 0; sig_idx < sig_num; sig_idx++)
          {
          if(Plot_Sig_Id->at(sig_num) == Plot_Sig_Id->at(sig_idx))
            {
            // warning duplicate signal
            *DebugFile << "Warning! Multiple plotting of same signal is not supported" << endl;
            *DebugFile << "signal " << sig_num << " is " 
                      << Plot_Sig_Id->at(sig_num)->GetName() << endl;
            *DebugFile << "signal " << sig_idx << " is " 
                      << Plot_Sig_Id->at(sig_idx)->GetName() << endl;
            Plot_Sig_Id->at(sig_num) = NULL;
            break;
            }
          }
        if(Plot_Sig_Id->at(sig_num) != NULL)
          {
          Plot_Sig_Id->at(sig_num)->SetupPlotFile(
                                  sig_id,
                                  start_time,
                                  stop_time,
                                  plot_decim_rate,
                                  count_vice_time,
                                  file_header_req);
          }
        } // if(sig_id != NULL)
      else
        {
        num_reject_sigs++;
        ErrorStream << "Requested plot signal '" << sig_name
                    << "' not found." << endl;
        }
      }//sig_idx
    Num_Plot_Sigs -= num_reject_sigs;
    }
  return;
}

void SignalPlotter::SetWakeAndSleep(  GenericSignal* sig_to_plot,
                                      int wakeup,
                                      int bedtime)
{
  int plot_idx;
*DebugFile << "In SignalPlotter::SetWakeAndSleep" << endl;
  for(plot_idx = 0; plot_idx < Num_Plot_Sigs; plot_idx++)
    {
    *DebugFile << "Plot_Sig_Id->at(plot_idx) = " << Plot_Sig_Id->at(plot_idx) << endl;
    *DebugFile << "sig_to_plot = " << sig_to_plot << endl;
    if( Plot_Sig_Id->at(plot_idx) == sig_to_plot )
      {
      Wakeup_Pass[plot_idx] = wakeup;
      Bedtime_Pass[plot_idx] = bedtime;
      return;
      }
    }
  *DebugFile << "Fatal error in SignalPlotter" << endl;
  exit(1);
}
//==========================================================
void SignalPlotter::CollectData(void)
{
  int sig_num;
  GenericSignal *sig_id;

  #ifdef _ENCLAVES
    for( sig_num = 0; sig_num < Num_Plot_Sigs; sig_num++)
      {
      sig_id = Plot_Sig_Id->at(sig_num);
      if(sig_id == NULL) continue;
      if(PassNumber > Bedtime_Pass[sig_num]) continue;
      if(PassNumber < Wakeup_Pass[sig_num]) continue;
      if(EnclaveNumber != sig_id->GetEnclave()) continue;
        //
        // else
        *DebugFile << "In Enclave " << EnclaveNumber 
                  << ", issuing plot data for "
                  << sig_id->GetName() << endl;
        sig_id->IssuePlotterData();
      }
  #endif
  //#ifdef _VAR_BLOCKS
  //if(PassNumber ==680)
  //{
  //sig_num=88;
  //}
    for( sig_num = 0; sig_num < Num_Plot_Sigs; sig_num++)
      {
      sig_id = Plot_Sig_Id->at(sig_num);
      if(sig_id == NULL) continue;
      //if(EnclaveNumber != sig_id->GetEnclave()) continue;
        //
        // else
        sig_id->IssuePlotterData();
      }
  //#endif
  return;
}
