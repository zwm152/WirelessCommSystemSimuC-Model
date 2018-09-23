//
//  File = psstream.h
//

#ifndef _PSSTREAM_H_
#define _PSSTREAM_H_

#include "psmodel.h"
//#include "f_cmplx.h"
//#include "d_cmplx.h"
#include "delay_modes.h"
#include "interp_modes.h"
#include <complex>
using namespace std;

class PracSimStream
{
public:
  PracSimStream(void);
  ~PracSimStream(void);

  //-------------------------------------------
  // add a new stream to the list of streams
  // comprising this composite pseudostream
  
  void SetReportFile( ofstream& new_report );

  //------------------------------------------------
  // overload insertion operator for each type of
  // output that might be issued by a PracSim model

  //PracSimStream& operator<< ( char *char_string );
  PracSimStream& operator<< ( const char *char_string );
  PracSimStream& operator<< ( char character );
  PracSimStream& operator<< ( float f_val );
  PracSimStream& operator<< ( double d_val );
  PracSimStream& operator<< ( int i_val );
  PracSimStream& operator<< ( long long_val );
  PracSimStream& operator<< ( std::complex<float> fc_val );
  PracSimStream& operator<< ( std::complex<double> dc_val );
  PracSimStream& operator<< ( void* voidptr_val );

  //----------------------------------------------------------------------
  // overloads of << for PracSimSpecific enum types

  friend PracSimStream& operator<<( PracSimStream&, const DELAY_MODE_T&);
  friend PracSimStream& operator<<( PracSimStream&, const INTERP_MODE_T&);

  //----------------------------------------------------
  // define signature that allows overloaded << to be
  // used for inserting manipulators into the pseudostream

  PracSimStream& operator<< (PracSimStream& (*_BRS) (PracSimStream&))
    { return((*_BRS)(*this));}

  //-----------------------------------------------------
  // methods for performing manipulator actions

  void InsertEndl(void);
  void InsertFlush(void);
  void InsertEnds(void);
  void InsertHex(void);
  void InsertOct(void);
  void InsertDec(void);
  void SetPrecision( int passed_int );
  void SetW( int passed_int );
  //void SetBase( int passed_int );
  //void SetFill( int passed_int );
  void SetIOSFlags( long new_flag );
  void ResetIOSFlags( long new_flag );
  void FillBlanks( int length );
  void FillBlanksRight( int length );
  void FillBlanksLeft( int length );
  void BadManipulator( void );

  //--------------------------------------------
  // flag functions

  long* flags(void);
  long flags(long* flag_buffer);
  long setf(long flag_bits);
  long setf(long flag_bits, long field);
  long unsetf(long unset_bits);
  void clear(int state=0);

  //-----------------------------------------
  //  format-related functions

  int width(void);
  int width(int len);
  char fill(void);
  char fill(char ch);
  int precision(int len);
  int precision(void);
private:
  int Num_Report_Files;
  ofstream* Report_File[5];
  
};

class pracsim_manip
{
private:
  int i;
  PracSimStream& (*f)(PracSimStream&, int);
public:
  pracsim_manip( PracSimStream& (*ff)(PracSimStream&, int),
                  int ii) : f(ff), i(ii) {};
  friend PracSimStream& operator<<( PracSimStream& rs, pracsim_manip& m);
};

class pracsim_manip_long
{
private:
  long l;
  PracSimStream& (*fl)(PracSimStream&, long);
public:
  pracsim_manip_long(PracSimStream& (*ff)(PracSimStream&, long),
                      long ll) : fl(ff), l(ll) {}
  friend PracSimStream& operator<<(
                PracSimStream& rs, pracsim_manip_long& m)
                {return m.fl(rs,m.l);}
};

//--------------------------------------------------
// manipulator invokers

  PracSimStream& _setprecision_(PracSimStream& results_stream, int passed_int);

  PracSimStream& _setw_(PracSimStream& results_stream, int passed_int);

  PracSimStream& _setbase_(PracSimStream& results_stream, int passed_int);

  PracSimStream& _setfill_(PracSimStream& results_stream, int passed_int);

  PracSimStream& _setiosflags_(PracSimStream& results_stream, long new_flag);

  PracSimStream& _resetiosflags_(PracSimStream& results_stream, long new_flag);

  PracSimStream& _fill_blanks_(PracSimStream& results_stream, int length);
  PracSimStream& _fill_blanks_right_(PracSimStream& results_stream, int length);
  PracSimStream& _fill_blanks_left_(PracSimStream& results_stream, int length);

  PracSimStream& endl(PracSimStream& results_stream);
  PracSimStream& flush(PracSimStream& results_stream);
  PracSimStream& ends(PracSimStream& results_stream);
  PracSimStream& hex(PracSimStream& results_stream);
  PracSimStream& oct(PracSimStream& results_stream);
  PracSimStream& dec(PracSimStream& results_stream);

;
#endif //_PSSTREAM_H_
