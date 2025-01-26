#include "menu.h"

void background(GameState *state) {
    static int mapGenerated = false;

    static PlayerState staticPlayerState = {{5.0, 5.0, 0.0}, 0.0, 0.0, 0.0, 0.0, 1, false, false, true, true};
    static MapState staticMapState = {10, 10, NULL, NULL, {0, 0, 0}};

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
}