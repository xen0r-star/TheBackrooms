#include "utils.h"


void calculateFPS(AppState *appState, PlayerState *playerState) {
    // Validate input parameters
    if (!appState || !playerState) {
        fprintf(stderr, "Error: Invalid parameters for FPS calculation\n");
        return;
    }

    // Calculate frame time in seconds
    clock_t currentTime = clock();
    double frameTime = (double)(currentTime - appState->startTime) / CLOCKS_PER_SEC;
    
    // Prevent division by zero and handle very small frame times
    if (frameTime <= 0.0) {
        frameTime = 0.016667; // Default to ~60 FPS (1/60 second)
    }
    
    // Update timing for next frame
    appState->startTime = currentTime;

    // Calculate frame-rate-independent movement speeds
    // This ensures consistent movement speed regardless of FPS
    playerState->playerMoveSpeed = playerState->moveSpeed * frameTime;
    playerState->playerRotateSpeed = playerState->rotateSpeed * frameTime;
    
    // Calculate and store FPS (frames per second)
    appState->fps = (int)(1.0 / frameTime);
    
    // Clamp FPS to reasonable range to prevent display issues
    if (appState->fps > 9999) {
        appState->fps = 9999;
    } else if (appState->fps < 1) {
        appState->fps = 1;
    }
}