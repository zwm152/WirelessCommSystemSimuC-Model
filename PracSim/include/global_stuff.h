//
//  File = global_stuff.h
//
//=========================================================
//  Global Stuff

#ifndef _GLOBAL_STUFF_H_
#define _GLOBAL_STUFF_H_
#include <stdlib.h> 
#include <iostream> 
#include <fstream>
#include <math.h>
//#include <direct.h>
#include "sigstuff.h"
#include "control_T.h"
#include "parmfile.h"
#include "syst_graph.h"
#include "sigplot.h"
#include "reports.h"
#include "exec.h"
#include "psstream.h"
   
ofstream LongReport;
ofstream ShortReport;
PracSimStream DetailedResults;
PracSimStream BasicResults;
PracSimStream ErrorStream;

char* DateString;
bool DateInFileNames;

PracSimModel *CommSystem;
SystemGraph CommSystemGraph;
Executive Exec;
PracSimModel *PrevModelConstr;
PracSimModel *ActiveModel;
SignalPlotter SigPlot;
int PassNumber;
int MaxPassNumber;
int EnclaveNumber;
int EnclaveOffset[10];

//#ifdef _DEBUG
// ofstream DebugFile("../"SIM_NAME"/"SIM_NAME".dbg", ios::out);
//#endif
//ParmFile ParmInput("../"SIM_NAME"/"SIM_NAME".dat");
//char SimName[] = SIM_NAME;
ParmFile *ParmInput;
ofstream *DebugFile;
char *SimName;
void SimulationStartup(char *sim_name, char *sim_title);
#endif
