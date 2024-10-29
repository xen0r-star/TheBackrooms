#ifndef MAP_H
#define MAP_H

#include "main.h"

extern int readMap(const char *filename, int ***map, int *mapWidth, int *mapHeight);
extern void generateMap(int map[mapSize][mapSize], int MAP_WIDTH, int MAP_HEIGHT, int percentObstacles);

#endif