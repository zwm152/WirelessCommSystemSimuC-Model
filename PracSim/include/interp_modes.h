//
// file = interp_modes.h
//

#ifndef _INTERP_MODES_H_
#define _INTERP_MODES_H_ 

typedef enum {
  INTERP_MODE_NONE,
  INTERP_MODE_LINEAR,
  INTERP_MODE_SINC,
  sizeof_INTERP_MODE_T
  } INTERP_MODE_T;

INTERP_MODE_T GetInterpModeParm(const char* parm_nam);

#endif
