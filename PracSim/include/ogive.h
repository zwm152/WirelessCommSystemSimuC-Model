//
//  File = ogive.h
//

#ifndef _OGIVE_H_
#define _OGIVE_H_

#include "psmodel.h"
#include "signal_T.h"
//#include "compress.h"
//#include "dirform1.h"
//using namespace std;

template <class T>
class OgiveBuilder : public PracSimModel
{
public:
  OgiveBuilder( char* instance_name,
                    PracSimModel* outer_model,
                    Signal<T>* in_sig );

  ~OgiveBuilder(void);
  void Initialize(void);
  int Execute(void);

private:
  int Block_Size;
  int Num_Segs_To_Tally;
  int Segs_In_Tally;
  Signal<T> *In_Sig;
  double Samp_Intvl;
  int *Hist_Bins;
  double Bin_Width;
  int Num_Bins;
  int Pts_In_Tally;
  int First_Pos_Bin;
  int Hold_Off;
  char *Hist_File_Name;
  ofstream *Hist_File;
  bool Halt_When_Completed;
  bool Processing_Completed;
  bool Positive_Only;
};

#endif //_OGIVE_H_
