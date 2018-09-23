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
#include "reports.h"

//================================================
// Global Stuff

extern ofstream LongReport;
extern ofstream ShortReport;
extern ofstream *DebugFile;
extern PracSimStream DetailedResults;
extern PracSimStream BasicResults;
extern PracSimStream ErrorStream;
extern char *DateString;


//================================================


void CreateReportFiles( char *report_name_root,
                        bool date_in_short_rpt_name,
                        bool date_in_full_rpt_name )
{
  char long_rpt_name[255];
  char short_rpt_name[255];
  char date_strm_buf[32];
  char date_str[32];

  //--------------------------------------------------------------------------
  // Build the date time string

  struct tm *time_struct;
  time_t clock_val;

  time( &clock_val );
  time_struct = localtime( &clock_val );

  strstream* date_stream = new strstream( date_strm_buf, 32, ios::out );
  (*date_stream)  << setfill('0') << setw(2) << (time_struct->tm_year - 100)
                  << setfill('0') << setw(2) << (1+time_struct->tm_mon) 
                  << setfill('0') << setw(2) << time_struct->tm_mday << "_"
                  << setfill('0') << setw(2) << time_struct->tm_hour 
                  << setfill('0') << setw(2) << time_struct->tm_min 
                  << setfill('0') << setw(2) << time_struct->tm_sec
                  << ends;

  strcpy( date_str, date_strm_buf);
  DateString = new char[32];
  strcpy( DateString, date_str);
  //-------------------------------------------------------------------------

  if( date_in_short_rpt_name )
    {
    strcpy( short_rpt_name, report_name_root);
    strcat( short_rpt_name, "_short_\0");
    strcat( short_rpt_name, date_str );
    strcat( short_rpt_name, ".txt\0");
    ShortReport.open(short_rpt_name, ios::out | ios::trunc);
    }
  else
    {
    strcpy( short_rpt_name, report_name_root);
    strcat( short_rpt_name, "_short.txt\0");
    ShortReport.open(short_rpt_name, ios::out | ios::trunc);
    }


  if(date_in_full_rpt_name)
    {
    strcpy( long_rpt_name, report_name_root);
    strcat( long_rpt_name, "_full_\0");
    strcat( long_rpt_name, date_str );
    strcat( long_rpt_name, ".txt\0");
    LongReport.open(long_rpt_name, ios::out | ios::trunc);
    }
  else
    {
    strcpy( long_rpt_name, report_name_root);
    strcat( long_rpt_name, "_full.txt\0");
    LongReport.open(long_rpt_name, ios::out | ios::trunc);
    }

  
  BasicResults.SetReportFile(ShortReport);
  BasicResults.SetReportFile(LongReport);
  DetailedResults.SetReportFile(LongReport);
  #ifdef _DEBUG
    BasicResults.SetReportFile(*DebugFile);
    ErrorStream.SetReportFile(*DebugFile);
  #endif
  ErrorStream.SetReportFile(ShortReport);
  ErrorStream.SetReportFile(LongReport);
  BasicResults << date_str << endl;
};
