#include "map.h"

void initializeMap(MapState *mapState, int width, int height, int percentObstacles) {
    if (mapState->map != NULL) {
        for (int i = 0; i < mapState->mapWidth; i++) {
            free(mapState->map[i]);
        }
        free(mapState->map);
    }

    if (mapState->mapDiscovered != NULL) {
        for (int i = 0; i < mapState->mapWidth; i++) {
            free(mapState->mapDiscovered[i]);
        }
        free(mapState->mapDiscovered);
    }


    mapState->mapWidth = width;
    mapState->mapHeight = height;


    mapState->map = malloc(width * sizeof(int *));
    mapState->mapDiscovered = malloc(width * sizeof(int *));
    for (int i = 0; i < width; i++) {
        mapState->map[i] = malloc(height * sizeof(int));
        mapState->mapDiscovered[i] = malloc(height * sizeof(int));
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                mapState->map[i][j] = 1;
            } else {
                mapState->map[i][j] = 0;
            }
            mapState->mapDiscovered[i][j] = 0;
        }
    }
    
    srand(time(NULL));
    int numObstacles = (width * height) / percentObstacles;

    for (int k = 0; k < numObstacles; k++) {
        int x = rand() % (width - 2) + 1;
        int y = rand() % (height - 2) + 1;
        mapState->map[y][x] = 2;
    }
}