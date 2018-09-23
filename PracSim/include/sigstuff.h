//
//  File = sigstuff.h
//
#ifndef _SIGSTUFF_H_
#define _SIGSTUFF_H_
#include "typedefs.h"

//#define BIT_SIGNAL(X) bitSignal* X=new bitSignal(#X);
#define BIT_SIGNAL(X) Signal<bit_t>* X=new Signal<bit_t>(#X);

#define BYTE_SIGNAL(X) Signal<byte_t>* X=new Signal<byte_t>(#X);
#define COMPLEX_SIGNAL(X) Signal<f_complex_t>* X=new Signal<f_complex_t>(#X);

//#define FLOAT_SIGNAL(X) floatSignal* X=new floatSignal(#X);
#define FLOAT_SIGNAL(X) Signal<float>* X=new Signal<float>(#X);

#define DOUBLE_SIGNAL(X) doubleSignal* X=new doubleSignal(#X);
#define INT_SIGNAL(X) Signal<int>* X=new Signal<int>(#X);
#define SHORT_SIGNAL(X) shortSignal* X=new shortSignal(#X);

#define GET_OUTPUT_PTR(X) X->GetRawOutputPtr(this)
#define GET_INPUT_PTR(X) X->GetRawInputPtr(this)

#define MAKE_OUTPUT(X) {  if(Nest_Depth == 1) \
  {Curr_Mod_Graph->InsertSignal( X, this, false);}}
//#define MAKE_OUTPUT(X) {  if(Nest_Depth == 1) \
//  {this->AddOutputSignal(X, false); \
//  Curr_Mod_Graph->InsertSignal( X, this, false);}}

#define MAKE_INPUT(X) { if(Nest_Depth == 1) \
  {Curr_Mod_Graph->InsertSignal( X, this, true); \
  X = X->AddConnection(  this, #X );}}
//#define MAKE_INPUT(X) { if(Nest_Depth == 1) \
//  {this->AddInputSignal(X, false); \
//  Curr_Mod_Graph->InsertSignal( X, this, true); \
//  X = X->AddConnection(  this, #X );}}

#define CHANGE_RATE(X,Y,Z) Curr_Mod_Graph->ChangeRate(X->GetId(), Y, Z, this)

#define SAME_RATE(X,Y) Curr_Mod_Graph->ChangeRate(X->GetId(), Y, 1.0, this)

#define SET_BLOCK_SIZE(X,Y) Curr_Mod_Graph->SetBlockSize(X->GetId(),Y);

//define SET_SAMP_RATE(X,Y) Curr_Mod_Graph->SetSampRate(X->GetId(),Y);
#define SET_SAMP_INTVL(X,Y) Curr_Mod_Graph->SetSampIntvl(X->GetId(),Y);

#define MAKE_SIG_FILE(X) ofstream X##_file(#X##".txt", ios::out)

#define ENABLE_MULTIRATE Curr_Mod_Graph->EnableMultirate();
#define ENABLE_CONST_INTERVAL Curr_Mod_Graph->EnableConstantInterval();

#endif      
