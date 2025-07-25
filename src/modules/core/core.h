#ifndef core_h
#define core_h

#include "../../main.h"

extern int castRay(const PlayerState playerState, MapState *mapState, float rayDirX, float rayDirY, float *distance, int *wallType, int *wallSide);

#endif