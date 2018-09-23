//
//  File = globals.h
//
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "psstream.h"
extern PracSimStream DetailedResults;
extern PracSimStream BasicResults;
extern PracSimStream ErrorStream;

#define REPORT_MODEL_NAME(X) {BasicResults << #X << endl;}

#endif // _GLOBALS_H_