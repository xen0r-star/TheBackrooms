#include "utils.h"

void showStateInterface(AppState *appState, const PlayerState playerState) {
    SDL_Renderer *renderer = appState->renderer;
    int screenWidth = appState->screenWidth;
    int screenHeight = appState->screenHeight;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
    SDL_Rect block = {10, 10, screenWidth / 3 - 10, screenHeight / 2 - 10};
    SDL_RenderFillRect(renderer, &block);

    static char fpsText[25];
    char text[25];
    if ((appState->startTime - appState->previousTime) * 1000 / CLOCKS_PER_SEC >= 500) {
        appState->previousTime = clock();
        sprintf(fpsText, "FPS: %.0f", appState->fps);
    }
    
    renderText(appState, 30, 30, 100, 25, fpsText, (SDL_Color){255, 255, 255, 255});

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect wallRect = {30, 70, screenWidth / 3 - 60, 2};
    SDL_RenderFillRect(renderer, &wallRect);

    sprintf(text, "Colision: %1d", playerState.collision);
    renderText(appState, 30, 80, 100, 25, text, (SDL_Color){255, 255, 255, 255});

    sprintf(text, "Texture: %1d", playerState.showTextures);
    renderText(appState, 30, 110, 92, 25, text, (SDL_Color){255, 255, 255, 255});

    sprintf(text, "Map: %1d", playerState.showMap);
    renderText(appState, 30, 140, 62, 25, text, (SDL_Color){255, 255, 255, 255});
}