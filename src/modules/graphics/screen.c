#include "graphics.h"

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

    return 0;
}