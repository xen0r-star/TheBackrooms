#ifndef castRay_h
#define castRay_h

#include "main.h"

extern int castRay(const PlayerState playerState, MapState *mapState, float angle, float *distance, int *wallType, int *wallSide);

#endif