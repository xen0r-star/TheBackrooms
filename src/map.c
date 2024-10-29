#include "map.h"

int readMap(const char *filename, int ***map, int *mapWidth, int *mapHeight) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    if (fscanf(file, "MAP_WIDTH %d\n", mapWidth) != 1) {
        fclose(file);
        perror("Error reading map width");
        return -1;
    }

    if (fscanf(file, "MAP_HEIGHT %d\n", mapHeight) != 1) {
        fclose(file);
        perror("Error reading map height");
        return -1;
    }

    *map = (int **)malloc(*mapHeight * sizeof(int *));
    for (int i = 0; i < *mapHeight; i++) {
        (*map)[i] = (int *)malloc(*mapWidth * sizeof(int));
    }

    for (int i = 0; i < *mapHeight; i++) {
        for (int j = 0; j < *mapWidth; j++) {
            if (fscanf(file, "%d", &(*map)[i][j]) != 1) {
                fclose(file);
                perror("Error reading map data");
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}


void generateMap(int map[mapSize][mapSize], int MAP_WIDTH, int MAP_HEIGHT, int percentObstacles) {
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