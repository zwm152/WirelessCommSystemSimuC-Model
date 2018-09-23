//
//  File = psstream.cpp
//

#include <stdlib.h>
#include <fstream>
#include <iomanip>
//#include <ios.h>
#include "psstream.h"

//=================================================
// constructor
PracSimStream::PracSimStream( )
{
  Num_Report_Files = 0;
}

//===============================================
// destructor
PracSimStream::~PracSimStream(){};

//================================================
//  Adds a file to the stream

void PracSimStream::SetReportFile( ofstream& new_report )
{
  Report_File[Num_Report_Files] = &new_report;
  Num_Report_Files++;
  return;
}

//=============================================
// insert character string

PracSimStream& PracSimStream::operator<< (const char* char_string)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << char_string;
    }
  return(*this);
}

//====================================================
// insert single character

PracSimStream& PracSimStream::operator<< (float f_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << f_val;
    }
  return(*this);

}

//====================================================
// insert double

PracSimStream& PracSimStream::operator<< (double d_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << d_val;
    }
  return(*this);
}


//====================================================
// insert int

PracSimStream& PracSimStream::operator<< (int i_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << i_val;
    }
  return(*this);
}

//====================================================
// insert long

PracSimStream& PracSimStream::operator<< (long long_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << long_val;
    }
  return(*this);
}

//====================================================
// insert float_complex

PracSimStream& PracSimStream::operator<< (std::complex<float> fc_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << fc_val.real() << ", " << fc_val.imag();
    }
  return(*this);
}

//====================================================
// insert double_complex

PracSimStream& PracSimStream::operator<< (std::complex<double> dc_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << dc_val.real() << ", " << dc_val.imag();
    }
  return(*this);
}

//====================================================
// insert void*

PracSimStream& PracSimStream::operator<< (void* voidptr_val)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << voidptr_val;
    }
  return(*this);
}

//====================================================
// insert endl manipulator

void PracSimStream::InsertEndl(void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << endl;
    }
  return;
}

//====================================================
// insert flush manipulator

void PracSimStream::InsertFlush(void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << flush;
    }
  return;
}

//====================================================
// insert ends manipulator

void PracSimStream::InsertEnds(void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << ends;
    }
  return;
}

//====================================================
// insert hex manipulator

void PracSimStream::InsertHex(void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << hex;
    }
  return;
}

//====================================================
// insert oct manipulator

void PracSimStream::InsertOct(void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << oct;
    }
  return;
}

//====================================================
// insert dec manipulator

void PracSimStream::InsertDec(void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << dec;
    }
  return;
}

//=================================================
void PracSimStream::SetPrecision( int passed_int)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << setprecision(passed_int);
    }
  return;
}


//=================================================
void PracSimStream::SetW( int passed_int)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << setw(passed_int);
    }
  return;
}

//=================================================
//void PracSimStream::SetBase( int passed_int)
//{
//  int i;
//  for(i=0; i<Num_Report_Files; i++)
//    {
//    (*(Report_File[i])) << setbase(passed_int);
//    }
//  return;
//}

#if 0
//=================================================
void PracSimStream::SetFill( int passed_int)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << setfill(passed_int);
    }
  return;
}
#endif

//=================================================
void PracSimStream::SetIOSFlags( long new_flag)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << setiosflags(new_flag);
    }
  return;
}


//=================================================
void PracSimStream::ResetIOSFlags( long new_flag)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << resetiosflags(new_flag);
    }
  return;
}

//=================================================
void PracSimStream::FillBlanks( int length)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << setw(length) << setfill(' ');
    }
  return;
}

//=================================================
void PracSimStream::FillBlanksRight( int length)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])).setf(ios::right);
    (*(Report_File[i])) << setw(length) << setfill(' ');
    }
  return;
}


//=================================================
void PracSimStream::FillBlanksLeft( int length)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])).setf(ios::left);
    (*(Report_File[i])) << setw(length) << setfill(' ');
    }
  return;
}


//=================================================
void PracSimStream::BadManipulator( void)
{
  int i;
  for(i=0; i<Num_Report_Files; i++)
    {
    (*(Report_File[i])) << "bad manipulator code detected" << endl;
    }
  exit(88);
}

//=================================================
long* PracSimStream::flags(void)
{
  int i;
  long* flag_buffer;

  flag_buffer = new long[Num_Report_Files];

  for(i=0; i<Num_Report_Files; i++)
    {
    flag_buffer[i] = (Report_File[i])->flags();
    }
  return(flag_buffer);
}

//=================================================
long PracSimStream::flags(long* flag_buffer)
{
  int i;
  long old_flag;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_flag = (Report_File[i])->flags(flag_buffer[i]);
    }
  return(old_flag);
}


//=================================================
long PracSimStream::setf(long flag_bits)
{
  int i;
  long old_flag;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_flag = (Report_File[i])->setf(flag_bits);
    }
  return(old_flag);
}


//=================================================
long PracSimStream::setf(long flag_bits, long field)
{
  int i;
  long old_flag;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_flag = (Report_File[i])->setf(flag_bits, field);
    }
  return(old_flag);
}

#if 0
//=================================================
long PracSimStream::unsetf(long unset_bits)
{
  int i;
  long old_flag;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_flag = (Report_File[i])->unsetf(unset_bits);
    }
  return(old_flag);
}
#endif
//=================================================
void PracSimStream::clear(int state)
{
  int i;

  for(i=0; i<Num_Report_Files; i++)
    {
    (Report_File[i])->clear(state);
    }
  return;
}


//=================================================
int PracSimStream::width(void)
{
  int i;
  int the_width;

  for(i=0; i<Num_Report_Files; i++)
    {
    the_width = (Report_File[i])->width();
    }
  return(the_width);
}


//=================================================
int PracSimStream::width(int len)
{
  int i;
  int old_width;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_width = (Report_File[i])->width(len);
    }
  return(old_width);
}


//=================================================
char PracSimStream::fill(void)
{
  int i;
  int the_fill;

  for(i=0; i<Num_Report_Files; i++)
    {
    the_fill = (Report_File[i])->fill();
    }
  return(the_fill);
}


//=================================================
char PracSimStream::fill(char ch)
{
  int i;
  int old_fill;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_fill = (Report_File[i])->fill(ch);
    }
  return(old_fill);
}


//=================================================
int PracSimStream::precision(void)
{
  int i;
  int the_precision;

  for(i=0; i<Num_Report_Files; i++)
    {
    the_precision = (Report_File[i])->precision();
    }
  return(the_precision);
}

//=================================================
int PracSimStream::precision(int len)
{
  int i;
  int old_precision;

  for(i=0; i<Num_Report_Files; i++)
    {
    old_precision = (Report_File[i])->precision(len);
    }
  return(old_precision);
}

//===================================================
//===================================================
//  manipulators for PracSimStream

//===========================================================
// setprecision
//
PracSimStream& _setprecision_( PracSimStream& results_stream,
                                int passed_int)
{
  results_stream.SetPrecision(passed_int);
  return(results_stream);
}

//===========================================================
// setw
//
PracSimStream& _setw_( PracSimStream& results_stream,
                                int passed_int)
{
  results_stream.SetW(passed_int);
  return(results_stream);
}

//===========================================================
// setbase
//
//PracSimStream& _setbase_( PracSimStream& results_stream,
//                                int passed_int)
//{
//  results_stream.SetBase(passed_int);
//  return(results_stream);
//}
#if 0
//===========================================================
// setfill
//
PracSimStream& _setfill_( PracSimStream& results_stream,
                                int passed_int)
{
  results_stream.SetFill(passed_int);
  return(results_stream);
}
#endif
//===========================================================
// setiosflags
//
PracSimStream& _setiosflags_( PracSimStream& results_stream,
                                long new_flag)
{
  results_stream.SetIOSFlags(new_flag);
  return(results_stream);
}

//===========================================================
// resetiosflags
//
PracSimStream& _resetiosflags_( PracSimStream& results_stream,
                                long new_flag)
{
  results_stream.ResetIOSFlags(new_flag);
  return(results_stream);
}

//===========================================================
// fill_blanks
//
PracSimStream& _fill_blanks_( PracSimStream& results_stream,
                                int length)
{
  results_stream.FillBlanks(length);
  return(results_stream);
}

//===========================================================
// fill_blanks_right
//
PracSimStream& _fill_blanks_right_( PracSimStream& results_stream,
                                int length)
{
  results_stream.FillBlanksRight(length);
  return(results_stream);
}

//===========================================================
// fill_blanks_left
//
PracSimStream& _fill_blanks_left_( PracSimStream& results_stream,
                                int length)
{
  results_stream.FillBlanksLeft(length);
  return(results_stream);
}

//===========================================================
// endl
//
PracSimStream& endl( PracSimStream& results_stream )
{
  results_stream.InsertEndl();
  return(results_stream);
}

//===========================================================
// flush
//
PracSimStream& flush( PracSimStream& results_stream )
{
  results_stream.InsertFlush();
  return(results_stream);
}

//===========================================================
// ends
//
PracSimStream& ends( PracSimStream& results_stream )
{
  results_stream.InsertEnds();
  return(results_stream);
}

//===========================================================
// hex
//
PracSimStream& hex( PracSimStream& results_stream )
{
  results_stream.InsertHex();
  return(results_stream);
}

//===========================================================
// oct
//
PracSimStream& oct( PracSimStream& results_stream )
{
  results_stream.InsertOct();
  return(results_stream);
}

//===========================================================
// dec
//
PracSimStream& dec( PracSimStream& results_stream )
{
  results_stream.InsertDec();
  return(results_stream);
}
//=======================================================================
PracSimStream& operator<<( PracSimStream& rs, pracsim_manip& m)
{
  return m.f(rs,m.i);
}

