//
// file = spect_calc_kinds.h
//

#ifndef _SPECT_CALC_KINDS_H_
#define _SPECT_CALC_KINDS_H_ 

typedef enum {
  SPECT_CALC_SAMPLE_SPECTRUM,
  SPECT_CALC_BARTLETT_PDGM,
  sizeof_KIND_OF_SPECT_CALC_T
  } KIND_OF_SPECT_CALC_T;

KIND_OF_SPECT_CALC_T GetKindOfSpectCalcParm(const char* parm_nam);

#endif
