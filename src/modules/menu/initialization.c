#include "menu.h"

int initializationMenu(GameState *state) {
    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;

    state->menu.playButton = (Button){
        {screenWidth / 2, 300 + 0 * (50 + 20), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    state->menu.achievementsButton = (Button){
        {screenWidth / 2, 300 + 1 * (50 + 20), 400, 50},
        {137, 136, 113, 127},
        "Succes"
    };

    state->menu.settingsButton = (Button){
        {screenWidth / 2, 300 + 2 * (50 + 20), 400, 50},
        {137, 136, 113, 127},
        "Parametres"
    };

    state->menu.exitButton = (Button){
        {screenWidth / 2, 300 + 3 * (50 + 20), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    state->menu.resumeGameButton = (Button){
        {screenWidth / 2, 300 + 0 * (50 + 20), 400, 50},
        {137, 136, 113, 127},
        "Reprendre"
    };

    state->menu.exitGameButton = (Button){
        {screenWidth / 2, 300 + 3 * (50 + 20), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter et sauvegarder"
    };

    state->menu.loadGame1 = (Button) {
        {screenWidth / 2, 100 + 145 * 0, screenWidth - 120, 125}, 
        {0, 0, 0, 255},
        "Load Game 1"
    };

    state->menu.loadGame2 = (Button) {
        {screenWidth / 2, 100 + 145 * 1, screenWidth - 120, 125}, 
        {0, 0, 0, 255},
        "Load Game 2"
    };

    state->menu.loadGame3 = (Button) {
        {screenWidth / 2, 100 + 145 * 2, screenWidth - 120, 125}, 
        {0, 0, 0, 255},
        "Load Game 3"
    };

    state->menu.launchGame = (Button) {
        {screenWidth / 2, 100 + 145 * 3, screenWidth - 120, 100}, 
        {0, 0, 0, 255},
        "Start Game"
    };

    state->menu.returnButton = (Button) {
        {screenWidth / 2, screenHeight - 25 - 20, screenWidth - 120, 50}, 
        {0, 0, 0, 255},
        "Retourner"
    };

    return 0;
}