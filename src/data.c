#include "data.h"

int saveData(const char *nameSave) {
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "saves/%s.dat", nameSave);


    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return 1;
    }

    fprintf(file, "PLAYER_X %f\n", player.x);
    fprintf(file, "PLAYER_Y %f\n", player.y);
    fprintf(file, "PLAYER_A %f\n", player.angle);

    fprintf(file, "\n");

    fprintf(file, "MAP_WIDTH  %d\n", mapWidth);
    fprintf(file, "MAP_HEIGHT %d\n", mapHeight);

    fprintf(file, "\n");

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            fprintf(file, "%d ", map[y][x]);
        }
        fprintf(file, "\n");
    }

    fprintf(file, "\n");

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            fprintf(file, "%d ", mapDiscovered[y][x]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}




int readData(const char *nameSave) {
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "saves/%s.dat", nameSave);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return 1;
    }

    if (fscanf(file, "PLAYER %f %f %f\n", &player.x, &player.y, &player.angle) != 3) {
        fclose(file);
        return 1;
    }

    if (fscanf(file, "MAP_WIDTH %d\n", &mapWidth) != 1) {
        fclose(file);
        return 1;
    }

    if (fscanf(file, "MAP_HEIGHT %d\n", &mapHeight) != 1) {
        fclose(file);
        return 1;
    }

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (fscanf(file, "%d", &map[y][x]) != 1) {
                fclose(file);
                return 1;
            }
        }
    }

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (fscanf(file, "%d", &mapDiscovered[y][x]) != 1) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}