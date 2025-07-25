#include "graphics.h"


#define PI 3.14159265358979323846

int initializationScreen(GameState *state) {
    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;

    // Création de la texture de rendu
    state->graphics.screenBuffersTexture = SDL_CreateTexture(
        state->app.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight
    );

    state->graphics.screenBuffers = (Uint32 *)malloc(screenWidth * screenHeight * sizeof(Uint32));
    if (!state->graphics.screenBuffers) {
        printf("Erreur: Allocation de screenBuffers échouée\n");
        return 1;
    }

    state->entityState.zBuffer = (float *)malloc(sizeof(float) * screenWidth);
    if (!state->entityState.zBuffer) {
        printf("Erreur: Allocation zBuffer échouée\n");
        return 1;
    }

    // Render Cache
    state->graphics.renderCache.fovRender = calculateFovRender(state->settings.fov);

    return 0;
}