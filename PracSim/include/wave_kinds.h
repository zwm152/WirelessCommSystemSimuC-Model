//
// file = wave_kinds.h
//

#ifndef _WAVE_KINDS_H_
#define _WAVE_KINDS_H_ 

typedef enum {
  PCM_NRZ,
  PCM_BIPHASE,
  PCM_DELAY_MOD,
  sizeof_PCM_WAVE_KIND_T
  } PCM_WAVE_KIND_T;

PCM_WAVE_KIND_T GetWaveKindParm(const char* parm_nam);

#endif
