#include "map.h"

void generateMap(int map[MAP_SIZE][MAP_SIZE], int MAP_WIDTH, int MAP_HEIGHT, int percentObstacles) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1) {
                map[i][j] = 1;
            } else {
                map[i][j] = 0;
            }
        }
    }
    
    srand(time(NULL));
    int numObstacles = (MAP_WIDTH * MAP_HEIGHT) / percentObstacles;

    for (int k = 0; k < numObstacles; k++) {
        int x = rand() % (MAP_WIDTH - 2) + 1;
        int y = rand() % (MAP_HEIGHT - 2) + 1;
        map[y][x] = 2;
    }
}