//
//  File = sim_startup.cpp
//

//#include "global_stuff.h" 
#include <direct.h>
#include <stdlib.h> 
#include <iostream> 
#include <fstream>
#include "parmfile.h"
#include "psmodel.h"
#include "reports.h"

//#include "gausrand.h"
//#include "bitgen.h"
//#include "bitwav.h"
//#include "clockgen.h"
//#include "add_gaus_noise.h"
//#include "qpskmod.h"
//#include "specanal.h"
//#include "qpskoptbitdem.h"
//#include "qpskdem.h"
//#include "ber_ctr.h"
//#include "qpsk_err_ctr.h"
//#include "siganchr.h"
//#include "qpsk_theory.h"
//'#include "ser_ctr.h"

//=========================================================
extern ParmFile *ParmInput;
extern ofstream *DebugFile;
extern PracSimModel *CommSystem;
extern PracSimModel *PrevModelConstr;
extern bool DateInFileNames;

//=========================================================

void SimulationStartup(char *sim_name, char *sim_title)
{
  char buffer[_MAX_PATH];
  char *filnam;

  /* Get the current working directory: */
  if( _getcwd( buffer, _MAX_PATH ) == NULL )
    cout << "_getcwd error" << endl;
  else
    cout << "%s\n" << buffer << endl;

  filnam = new char[_MAX_PATH-32];
  strcpy(filnam, sim_name);
  strcat(filnam,".dat\0");
  ParmInput = new ParmFile(filnam);
  #ifdef _DEBUG
    strcpy(filnam,"../\0");
    strcat(filnam, sim_name);
    strcat(filnam, "/\0");
    strcat(filnam, sim_name);
    strcat(filnam, ".dbg\0");
    DebugFile = new ofstream(filnam, ios::out);
  #endif

    //return 0;

  CommSystem = new PracSimModel(0, "CommSystem\0");
  PrevModelConstr = NULL;
  ParmInput->FindBlock("system\0");

  bool Date_In_Short_Rpt_Name;
  bool Date_In_Full_Rpt_Name;
  Date_In_Short_Rpt_Name = ParmInput->GetBoolParm("Date_In_Short_Rpt_Name\0");
  Date_In_Full_Rpt_Name = ParmInput->GetBoolParm("Date_In_Full_Rpt_Name\0");

  DateInFileNames = Date_In_Short_Rpt_Name || Date_In_Full_Rpt_Name;

  CreateReportFiles( sim_name,
                     Date_In_Short_Rpt_Name,
                     Date_In_Full_Rpt_Name );

  int title_len;
  title_len = int(strlen(sim_title));
  int ich;
  cout << sim_title << endl;
  for(ich=0; ich<title_len; ich++)
    {
    cout << "=";
    }
  cout << "\n" << endl;

  BasicResults << sim_title << endl;
  for(ich=0; ich<title_len; ich++)
    {
    BasicResults << "=";
    }
  BasicResults << "\n" << endl;

  #ifdef _DEBUG
    *DebugFile << sim_title << endl;
    for(ich=0; ich<title_len; ich++)
      {
      *DebugFile << "=";
      }
    *DebugFile << "\n" << endl;
  #endif
}
