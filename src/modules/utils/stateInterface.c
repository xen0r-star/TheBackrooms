#include "utils.h"

void showStateInterface(AppState *appState, const PlayerState playerState) {
    SDL_Renderer *renderer = appState->renderer;
    int screenWidth = appState->screenWidth;
    int screenHeight = appState->screenHeight;
    int interfaceWidth = screenWidth / 3 - 10;
    int interfaceHeight = screenHeight / 2 - 10;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
    SDL_Rect block = {10, 10, interfaceWidth, interfaceHeight};
    SDL_RenderFillRect(renderer, &block);

    static char fpsText[25];
    char text[25];
    if ((appState->startTime - appState->previousTime) * 1000 / CLOCKS_PER_SEC >= 500) {
        appState->previousTime = clock();
        sprintf(fpsText, "FPS: %.0f", appState->fps);
    }
    
    renderText(appState, LEFT, 30, 40, fpsText, (Color){255, 255, 255, 255}, TEXT_XS);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect wallRect = {30, 70, interfaceWidth - 50, 2};
    SDL_RenderFillRect(renderer, &wallRect);

    sprintf(text, "Colision: %1d", playerState.collision);
    renderText(appState, LEFT, 30, 90, text, (Color){255, 255, 255, 255}, TEXT_XS);

    sprintf(text, "Texture: %1d", playerState.showTextures);
    renderText(appState, LEFT, 30, 120, text, (Color){255, 255, 255, 255}, TEXT_XS);

    sprintf(text, "Map: %1d", playerState.showMap);
    renderText(appState, LEFT, 30, 150, text, (Color){255, 255, 255, 255}, TEXT_XS);
}