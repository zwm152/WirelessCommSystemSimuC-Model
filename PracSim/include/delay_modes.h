//
// file = delay_modes.h
//

#ifndef _DELAY_MODES_H_
#define _DELAY_MODES_H_ 

typedef enum {
  DELAY_MODE_NONE,
  DELAY_MODE_FIXED,
  DELAY_MODE_DYNAMIC,
  DELAY_MODE_GATED,
  sizeof_DELAY_MODE_T
  } DELAY_MODE_T;

DELAY_MODE_T GetDelayModeParm(const char* parm_nam);

#endif
