#include "menu.h"

int initializationMenu(GameState *state) {
    state->menu.backgroundTexture = IMG_LoadTexture(state->app.renderer, BACKGROUND_FILE);
    
    int screenWidth = state->app.screenWidth;

    state->menu.playButton = (Button){
        {(screenWidth - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    state->menu.achievementsButton = (Button){
        {(screenWidth - 400) / 2, (275 + 1 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Succès"
    };

    state->menu.settingsButton = (Button){
        {(screenWidth - 400) / 2, (275 + 2 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Paramètres"
    };

    state->menu.exitButton = (Button){
        {(screenWidth - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    state->menu.resumeGameButton = (Button){
        {(screenWidth - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Reprendre"
    };

    state->menu.exitGameButton = (Button){
        {(screenWidth - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter et sauvegarder"
    };

    state->menu.loadGame1 = (Button) {
        {50, 50 + 125 * 0 + 20 * 0, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 1"
    };

    state->menu.loadGame2 = (Button) {
        {50, 50 + 125 * 1 + 20 * 1, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 2"
    };

    state->menu.loadGame3 = (Button) {
        {50, 50 + 125 * 2 + 20 * 2, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 3"
    };

    state->menu.launchGame = (Button) {
        {50, 50 + 125 * (4 - 1) + 20 * (4 - 1), screenWidth - 100 - 15, 100}, 
        {0, 0, 0, 255},
        "Start Game"
    };

    return 0;
}