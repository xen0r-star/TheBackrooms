#include "data.h"

int readData(GameState *state, const char *nameSave) {
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "saves/%s.dat", nameSave);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return 1;
    }

    if (fscanf(file, "PLAYER %f %f %f\n", &state->playerState.player.x, &state->playerState.player.y, &state->playerState.player.angle) != 3) {
        fclose(file);
        return 1;
    }

    if (fscanf(file, "MAP_WIDTH %d\n", &state->mapState.mapWidth) != 1) {
        fclose(file);
        return 1;
    }

    if (fscanf(file, "MAP_HEIGHT %d\n", &state->mapState.mapHeight) != 1) {
        fclose(file);
        return 1;
    }

    for (int y = 0; y < state->mapState.mapHeight; y++) {
        for (int x = 0; x < state->mapState.mapWidth; x++) {
            if (fscanf(file, "%d", &state->mapState.map[y][x]) != 1) {
                fclose(file);
                return 1;
            }
        }
    }

    for (int y = 0; y < state->mapState.mapHeight; y++) {
        for (int x = 0; x < state->mapState.mapWidth; x++) {
            if (fscanf(file, "%d", &state->mapState.mapDiscovered[y][x]) != 1) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}