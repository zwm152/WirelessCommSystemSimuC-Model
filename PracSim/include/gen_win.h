//
//  File = gen_win.h
//

#ifndef _GEN_WIN_H_
#define _GEN_WIN_H_ 

#include <iostream>
#include <fstream>
using namespace std;
class GenericWindow
{
public:
  GenericWindow( int length );
  double GetDataWinCoeff( int samp_indx);
  void NormalizeWindow( void );
  double* GetDataWindow( void );
  double* GetHalfLagWindow( void );
  int GetNumTaps( void );
protected:
  void Initialize( int length );
  int Length;
  int Half_Length;
  double *Half_Lag_Win;
  double *Lag_Win;
  double *Data_Win;
};

#endif
  