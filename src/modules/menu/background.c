#include "menu.h"

void background(GameState *state, BackgroundType backgroundType) {
    if (backgroundType == BACKGROUND_MENU) {
        static int mapGenerated = false;

        static PlayerState staticPlayerState = {{5.0, 5.0, 0.0}, 0.0, 0.0, 0.0, 0.0, 1, false, false, true, true};
        static MapState staticMapState = {10, 10, NULL, NULL, 0};

        if (!mapGenerated) {
            initializeMap(&staticMapState, staticMapState.mapWidth, staticMapState.mapHeight, 5);
            mapGenerated = true;
        }

        staticPlayerState.player.angle += 0.025;
        staticPlayerState.player.angle = (staticPlayerState.player.angle > 360) ? 0 : staticPlayerState.player.angle;

        GameState gameStateCopy = {
            .app = state->app,
            .playerState = staticPlayerState,
            .mapState = staticMapState,
            .graphics = state->graphics,
            .settings = state->settings
        };

        renderScene(&gameStateCopy);
    } else if (backgroundType == BACKGROUND_GAME) {
        SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, 
            state->graphics.screenBuffers, 
            state->app.screenWidth * sizeof(Uint32)
        );
        SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);

        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 190);
        SDL_Rect block = {0, 0, state->app.screenWidth, state->app.screenHeight};
        SDL_RenderFillRect(state->app.renderer, &block);
    }
}