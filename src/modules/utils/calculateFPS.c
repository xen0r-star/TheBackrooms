#include "utils.h"

void calculateFPS(AppState *appState, PlayerState *playerState) {
    appState->fps = (double) (clock() - appState->startTime) / CLOCKS_PER_SEC;
    appState->startTime = clock();

    playerState->playerMoveSpeed = playerState->moveSpeed * appState->fps;
    playerState->playerRotateSpeed = playerState->rotateSpeed * appState->fps;
    appState->fps = (int)(1. / appState->fps);
}