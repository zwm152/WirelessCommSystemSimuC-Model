//
//  File = code_gain_sim.cpp
//
#include <stdlib.h> 
#include <iostream> 
#include <fstream>
#include <time.h>
#include <strstream>
#include <iomanip>
#include "parmfile.h"
#include "psstream.h"
#include "model_error.h"

//================================================
// Global Stuff

extern ofstream LongReport;
extern ofstream ShortReport;
extern ofstream *DebugFile;
extern PracSimStream DetailedResults;
extern PracSimStream BasicResults;
extern PracSimStream ErrorStream;
extern PracSimModel *ActiveModel;


//================================================


void PsModelError( bool is_fatal,
                 char* msg_text)
{
  BasicResults << endl << endl;
  BasicResults << "*******************************************"
               << endl << endl;
  if(is_fatal)
    {
    BasicResults << "Fatal PracSim Error in instance ";
    BasicResults << ActiveModel->GetInstanceName() << " of model " ;
    BasicResults << ActiveModel->GetModelName() << endl;
    BasicResults << msg_text << endl;
    BasicResults << endl << "Execution Terminated" << endl;

    #ifdef _CONSOLE
      cout << "Fatal PracSim Error in instance ";
      cout << ActiveModel->GetInstanceName() << " of model " ;
      cout << ActiveModel->GetModelName() << endl;
      cout << msg_text << endl;
      cout << endl << "Execution Terminated" << endl;
    #endif
    exit(-1);
    }
  else
    {
    BasicResults << "PracSim Warning in instance ";
    BasicResults << ActiveModel->GetInstanceName() << " of model " ;
    BasicResults << ActiveModel->GetModelName() << endl;
    BasicResults << msg_text << endl;
    BasicResults << endl << "Execution Continues . . ." << endl;

    #ifdef _CONSOLE
      cout << "PracSim Warning in instance ";
      cout << ActiveModel->GetInstanceName() << " of model " ;
      cout << ActiveModel->GetModelName() << endl;
      cout << msg_text << endl;
      cout << endl << "Execution Continues . . ." << endl;
    #endif
    }
  return;
};
