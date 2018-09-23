//
//  File = parmfile.h
//

#ifndef _PARMFILE_H_
#define _PARMFILE_H_
#include "globals.h"
#define OPEN_PARM_BLOCK {ParmInput->FindBlock(instance_name);\
                          BasicResults << instance_name << endl;}
#define GET_INT_PARM(X) {X = ParmInput->GetIntParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_INT_PARM_ARRAY(X,N) {X = ParmInput->GetIntParmArray(#X##"\0",X,N);\
                         BasicResults << "   " << #X##" = " << X[0] << endl;}
#define GET_BOOL_PARM(X) {X = ParmInput->GetBoolParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_LONG_PARM(X) {X = ParmInput->GetLongParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_FLOAT_PARM(X) {X = ParmInput->GetFloatParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_DOUBLE_PARM(X) {X = ParmInput->GetDoubleParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
//#define GET_DOUBLE_PARM_ARRAY(X,N) {X = ParmInput->GetDoubleParmArray(#X##"\0",X,N);\
//                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_DOUBLE_PARM_ARRAY(X,N) {X = ParmInput->GetDoubleParmArray(#X##"\0",X,N);\
                         for(int __i=0; __i<N; __i++) {\
                         BasicResults << "   " << #X##"[" << __i << "] = " << *(((double*)X)+__i) << endl;}}

#define GET_STRING_PARM(X) {X = ParmInput->GetStringParm(#X);\
                           BasicResults << "   " << #X##" = " << X << endl;}

#define GET_INT_KRNL_PARM(X) {Kernel->X = ParmInput->GetIntParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_BOOL_KRNL_PARM(X) {Kernel->X = ParmInput->GetBoolParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_LONG_KRNL_PARM(X) {Kernel->X = ParmInput->GetLongParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_FLOAT_KRNL_PARM(X) {Kernel->X = ParmInput->GetFloatParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}
#define GET_DOUBLE_KRNL_PARM(X) {Kernel->X = ParmInput->GetDoubleParm(#X);\
                         BasicResults << "   " << #X##" = " << X << endl;}

class ParmFile
{
public:
  ParmFile( const char* input_file_name );
  ~ParmFile(void);
  void FindBlock(const char* block_nam);
  void ParmFile::RestartBlock(void);
  int GetParmStr(const char* parm_nam, char *ret_str);
  int GetParmArrayStr(const char* parm_nam, char* ret_str);
  char* GetStringParm(const char* parm_nam);
  int GetIntParm(const char* parm_nam);
  long GetLongParm(const char* parm_nam);
  float GetFloatParm(const char* parm_nam);
  double GetDoubleParm(const char* parm_nam);
  bool GetBoolParm(const char* parm_nam);

  int* GetIntParmArray(  const char* parm_nam, 
                        int* array_ptr,
                        const int array_len);
  double* GetDoubleParmArray(  const char* parm_nam, 
                        double* array_ptr,
                        const int array_len);

  void GetPlotSpec( char *sig_name,
                    double *start_time,
                    double *stop_time,
                    int *plot_decim_rate,
                    bool *count_vice_time,
                    bool *file_header_req);
private:
  ifstream *Input_File;
  char *Input_File_Name;
  char *Block_Name;
  
};

#endif
