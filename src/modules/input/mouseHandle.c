#include "input.h"

void mouseHandle(GameState *state, const SDL_Event event) {
    int displayMenu = state->menu.displayMenu;

    if (event.type == SDL_MOUSEMOTION && displayMenu == MENU_NONE) {
        int mouseX = event.motion.xrel;

        state->playerState.player.angle += mouseX * state->settings.sensitivity;
        if (state->playerState.player.angle < 0)    state->playerState.player.angle += 360;
        if (state->playerState.player.angle >= 360) state->playerState.player.angle -= 360;

    } else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0 && state->playerState.selectFrame > 1)       state->playerState.selectFrame -= 1;
        else if (event.wheel.y < 0 && state->playerState.selectFrame < 5)  state->playerState.selectFrame += 1;

    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && displayMenu != MENU_NONE) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        switch (displayMenu) {
            case MENU_MAIN:
                if (clickedButton(state->menu.playButton, mouseX, mouseY)) {
                    state->menu.displayMenu = MENU_LOAD;
                } else if (clickedButton(state->menu.achievementsButton, mouseX, mouseY)) {
                    state->menu.displayMenu = MENU_ACHIEVEMENTS;
                } else if (clickedButton(state->menu.settingsButton, mouseX, mouseY)) {
                    state->menu.displayMenu = MENU_SETTINGS;
                } else if (clickedButton(state->menu.exitButton, mouseX, mouseY)) {
                    state->app.running = false;
                }
                break;
            
            case MENU_LOAD:
                if (clickedButton(state->menu.loadGame1, mouseX, mouseY)) {
                    state->mapState.typeLaunchGame[0] = BUTTON_SELECTED;
                } else if (clickedButton(state->menu.loadGame2, mouseX, mouseY)) {
                    state->mapState.typeLaunchGame[1] = BUTTON_SELECTED;
                } else if (clickedButton(state->menu.loadGame3, mouseX, mouseY)) {
                    state->mapState.typeLaunchGame[2] = BUTTON_SELECTED; 
                } else if (clickedButton(state->menu.launchGame, mouseX, mouseY)) {
                    state->menu.displayMenu = MENU_NONE;
                }
                break;
            
            case MENU_BREAK:
                if (clickedButton(state->menu.resumeGameButton, mouseX, mouseY)) {
                    state->menu.displayMenu = MENU_NONE;
                } else if (clickedButton(state->menu.achievementsButton, mouseX, mouseY)) { 
                    state->menu.displayMenu = MENU_ACHIEVEMENTS;
                } else if (clickedButton(state->menu.settingsButton, mouseX, mouseY)) {     
                    state->menu.displayMenu = MENU_SETTINGS;
                } else if (clickedButton(state->menu.exitGameButton, mouseX, mouseY)) {
                    state->menu.displayMenu = MENU_MAIN;
                    saveData(state, "Save1");
                }
                break;
            
            default:
                break;
        }
    }
}