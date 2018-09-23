//
// file = window_shapes.h
//

#ifndef _WINDOW_SHAPES_H_
#define _WINDOW_SHAPES_H_ 

typedef enum {
  WINDOW_SHAPE_TRIANGULAR,
  WINDOW_SHAPE_HAMMING,
  WINDOW_SHAPE_HANN,
  sizeof_WINDOW_SHAPE_T
  } WINDOW_SHAPE_T;

WINDOW_SHAPE_T GetWindowShapeParm(const char* parm_nam);

#endif
