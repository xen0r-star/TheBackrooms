#ifndef map_H
#define map_H

#include "../../main.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


typedef struct {
    int width;
    int height;
    const char** tiles;
    int maxOccurrences;
} RoomPattern;


extern void generateMap(MapState *mapState, int width, int height);
extern void spawnPlayerFromMap(PlayerState *playerState, MapState *mapState);

#endif