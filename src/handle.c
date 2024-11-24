#include "handle.h"
#include "menu.h"
#include "data.h"

void keyboardDown(GameState *state, const SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_m:
                state->playerState.showMap      = !state->playerState.showMap;
                break;
            case SDLK_f:
                state->playerState.showState    = !state->playerState.showState;
                break;
            case SDLK_t:
                state->playerState.showTextures = !state->playerState.showTextures;
                break;
            case SDLK_c:
                state->playerState.collision    = !state->playerState.collision;
                break;
            case SDLK_ESCAPE:
                if (state->menu.displayMenu == MENU_NONE) state->menu.displayMenu = MENU_BREAK;
                break;
            default:
                break;
        }
    }
}



void keyboardInput(GameState *state) {
    // Avancer
    if (state->app.keystate[SDL_SCANCODE_UP] || state->app.keystate[26]) {
        float newX = state->playerState.player.x + cos(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed;
        float newY = state->playerState.player.y + sin(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed;
        if (state->playerState.collision) { // Vérifie les collisions avec les murs
            if (state->mapState.map[(int)newY][(int)newX] == 0) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        } else {
            if (state->mapState.map[(int)newY][(int)newX] != 1) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        }
    }

    // Reculer
    if (state->app.keystate[SDL_SCANCODE_DOWN] || state->app.keystate[22]) {
        float newX = state->playerState.player.x - cos(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed;
        float newY = state->playerState.player.y - sin(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed;
        if (state->playerState.collision) { // Vérifie les collisions avec les murs
            if (state->mapState.map[(int)newY][(int)newX] == 0) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        } else {
            if (state->mapState.map[(int)newY][(int)newX] != 1) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        }
    }

    if (state->app.keystate[4]) {
        float newX = state->playerState.player.x + sin(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed / 2;
        float newY = state->playerState.player.y - cos(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed / 2;
        if (state->playerState.collision) { // Vérifie les collisions avec les murs
            if (state->mapState.map[(int)newY][(int)newX] == 0) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        } else {
            if (state->mapState.map[(int)newY][(int)newX] != 1) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        }
    }

    if (state->app.keystate[7]) {
        float newX = state->playerState.player.x - sin(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed / 2;
        float newY = state->playerState.player.y + cos(state->playerState.player.angle * M_PI / 180) * state->playerState.playerMoveSpeed / 2;
        if (state->playerState.collision) { // Vérifie les collisions avec les murs
            if (state->mapState.map[(int)newY][(int)newX] == 0) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        } else {
            if (state->mapState.map[(int)newY][(int)newX] != 1) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        }
    }

    if (state->app.keystate[SDL_SCANCODE_LEFT]) {
        state->playerState.player.angle -= state->playerState.playerRotateSpeed;
        if (state->playerState.player.angle < 0) state->playerState.player.angle += 360;
    }

    if (state->app.keystate[SDL_SCANCODE_RIGHT]) {
        state->playerState.player.angle += state->playerState.playerRotateSpeed;
        if (state->playerState.player.angle >= 360) state->playerState.player.angle -= 360;
    }
}


void mouseHandle(GameState *state, const SDL_Event event) {
    int displayMenu = state->menu.displayMenu;

    if (event.type == SDL_MOUSEMOTION && displayMenu == MENU_NONE) {
        int mouseX = event.motion.xrel;

        state->playerState.player.angle += mouseX * state->playerState.sensitivity;
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
                printf("main\n");
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


void controllerDown(GameState *state, const SDL_Event event) {
    if (event.type == SDL_CONTROLLERBUTTONDOWN) {
        switch (event.cbutton.button) {
            case SDL_CONTROLLER_BUTTON_Y:     
                state->playerState.showMap = !state->playerState.showMap; 
                break;
            case SDL_CONTROLLER_BUTTON_BACK:  
                state->playerState.showTextures = !state->playerState.showTextures; 
                break;
            case SDL_CONTROLLER_BUTTON_START: 
                state->playerState.showState = !state->playerState.showState; 
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                if (state->menu.displayMenu == MENU_NONE && state->playerState.selectFrame < 10) state->playerState.selectFrame += 1;
                break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                if (state->menu.displayMenu == MENU_NONE && state->playerState.selectFrame > 1)  state->playerState.selectFrame -= 1;
                break;
            default: break;
        }
    }

}



void controllerInput(GameState *state) {
    SDL_GameControllerUpdate();


    Sint16 axisLX = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_LEFTX);
    Sint16 axisLY = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_LEFTY);
    Sint16 axisRX = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_RIGHTX);
    // Sint16 axisRY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

    if (abs(axisLY) > 8000) {
        float newX = state->playerState.player.x - cos(state->playerState.player.angle * M_PI / 180) * (axisLY / 32767.0f) * state->playerState.playerMoveSpeed;
        float newY = state->playerState.player.y - sin(state->playerState.player.angle * M_PI / 180) * (axisLY / 32767.0f) * state->playerState.playerMoveSpeed;
        if (state->playerState.collision) {
            if (state->mapState.map[(int)newY][(int)newX] == 0) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        } else {
            if (state->mapState.map[(int)newY][(int)newX] != 1) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        }
    }

    if (abs(axisLX) > 8000) {
        float newX = state->playerState.player.x - sin(state->playerState.player.angle * M_PI / 180) * (axisLX / 32767.0f) * state->playerState.playerMoveSpeed / 2;
        float newY = state->playerState.player.y + cos(state->playerState.player.angle * M_PI / 180) * (axisLX / 32767.0f) * state->playerState.playerMoveSpeed / 2;
        if (state->playerState.collision) {
            if (state->mapState.map[(int)newY][(int)newX] == 0) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        } else {
            if (state->mapState.map[(int)newY][(int)newX] != 1) {
                state->playerState.player.x = newX;
                state->playerState.player.y = newY;
            }
        }
    }

    if (abs(axisRX) > 8000) {
        state->playerState.player.angle += (axisRX / 32767.0f) * state->playerState.playerRotateSpeed;
        if (state->playerState.player.angle < 0)    state->playerState.player.angle += 360;
        if (state->playerState.player.angle >= 360) state->playerState.player.angle -= 360;
    }
}