//
// file = adv_modes.h
//

#ifndef _ADV_MODES_H_
#define _ADV_MODES_H_ 

typedef enum {
  ADVANCE_MODE_NONE,
  ADVANCE_MODE_FIXED,
  ADVANCE_MODE_DYNAMIC,
  ADVANCE_MODE_GATED,
  sizeof_ADVANCE_MODE_T
  } ADVANCE_MODE_T;

ADVANCE_MODE_T GetAdvanceModeParm(const char* parm_nam);

#endif
