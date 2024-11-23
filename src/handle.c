#include "handle.h"
#include "menu.h"
#include "data.h"

void keyboardDown(const SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_m:
                showMap = !showMap;
                break;
            case SDLK_f:
                showState = !showState;
                break;
            case SDLK_t:
                showTextures = !showTextures;
                break;
            case SDLK_c:
                colision = !colision;
                break;
            case SDLK_ESCAPE:
                if (displayMenu == MENU_NONE) displayMenu = MENU_BREAK;
                break;
            default:
                break;
        }
    }
}



void keyboardInput() {
    // Avancer
    if (keystate[SDL_SCANCODE_UP] || keystate[26]) {
        float newX = player.x + cos(player.angle * M_PI / 180) * playerMoveSpeed;
        float newY = player.y + sin(player.angle * M_PI / 180) * playerMoveSpeed;
        if (colision) { // Vérifie les collisions avec les murs
            if (map[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        } else {
            if (map[(int)newY][(int)newX] != 1) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    // Reculer
    if (keystate[SDL_SCANCODE_DOWN] || keystate[22]) {
        float newX = player.x - cos(player.angle * M_PI / 180) * playerMoveSpeed;
        float newY = player.y - sin(player.angle * M_PI / 180) * playerMoveSpeed;
        if (colision) { // Vérifie les collisions avec les murs
            if (map[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        } else {
            if (map[(int)newY][(int)newX] != 1) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    if (keystate[4]) {
        float newX = player.x + sin(player.angle * M_PI / 180) * playerMoveSpeed / 2;
        float newY = player.y - cos(player.angle * M_PI / 180) * playerMoveSpeed / 2;
        if (colision) { // Vérifie les collisions avec les murs
            if (map[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        } else {
            if (map[(int)newY][(int)newX] != 1) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    if (keystate[7]) {
        float newX = player.x - sin(player.angle * M_PI / 180) * playerMoveSpeed / 2;
        float newY = player.y + cos(player.angle * M_PI / 180) * playerMoveSpeed / 2;
        if (colision) { // Vérifie les collisions avec les murs
            if (map[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        } else {
            if (map[(int)newY][(int)newX] != 1) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    if (keystate[SDL_SCANCODE_LEFT]) {
        player.angle -= playerRotateSpeed;
        if (player.angle < 0) player.angle += 360;
    }

    if (keystate[SDL_SCANCODE_RIGHT]) {
        player.angle += playerRotateSpeed;
        if (player.angle >= 360) player.angle -= 360;
    }
}


void mouseHandle(const SDL_Event event) {
    if (event.type == SDL_MOUSEMOTION && displayMenu == MENU_NONE) {
        int mouseX = event.motion.xrel;

        player.angle += mouseX * sensitivity;
        if (player.angle < 0) player.angle += 360;
        if (player.angle >= 360) player.angle -= 360;
    } else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0 && selectFrame > 1)       selectFrame -= 1;
        else if (event.wheel.y < 0 && selectFrame < 5) selectFrame += 1;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && displayMenu != MENU_NONE) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        switch (displayMenu) {
            case MENU_MAIN:
                if (clickedButton(playButton, mouseX, mouseY)) {
                    displayMenu = MENU_LOAD;
                } else if (clickedButton(achievementsButton, mouseX, mouseY)) {
                    displayMenu = MENU_ACHIEVEMENTS;
                } else if (clickedButton(settingsButton, mouseX, mouseY)) {
                    displayMenu = MENU_SETTINGS;
                } else if (clickedButton(exitButton, mouseX, mouseY)) {
                    running = false;
                }
                break;
            
            case MENU_LOAD:
                if (clickedButton(loadGame1, mouseX, mouseY)) {
                    typeLaunchGame[0] = BUTTON_SELECTED;
                } else if (clickedButton(loadGame2, mouseX, mouseY)) {
                    typeLaunchGame[1] = BUTTON_SELECTED;
                } else if (clickedButton(loadGame3, mouseX, mouseY)) {
                    typeLaunchGame[2] = BUTTON_SELECTED;
                } else if (clickedButton(launchGame, mouseX, mouseY)) {
                    displayMenu = MENU_NONE;
                }
                break;
            
            case MENU_BREAK:
                if (clickedButton(resumeGameButton, mouseX, mouseY)) {
                    displayMenu = MENU_NONE;
                } else if (clickedButton(achievementsButton, mouseX, mouseY)) { 
                    displayMenu = MENU_ACHIEVEMENTS;
                } else if (clickedButton(settingsButton, mouseX, mouseY)) {     
                    displayMenu = MENU_SETTINGS;
                } else if (clickedButton(extiGameButton, mouseX, mouseY)) {
                    displayMenu = MENU_MAIN;
                    saveData("Save1");
                }
                break;
            
            default:
                break;
        }
    }
}


void controllerDown(const SDL_Event event) {
    if (event.type == SDL_CONTROLLERBUTTONDOWN) {
        switch (event.cbutton.button) {
            case SDL_CONTROLLER_BUTTON_Y:     
                showMap = !showMap; 
                break;
            case SDL_CONTROLLER_BUTTON_BACK:  
                showTextures = !showTextures; 
                break;
            case SDL_CONTROLLER_BUTTON_START: 
                showState = !showState; 
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                if (displayMenu == MENU_NONE && selectFrame < 10) selectFrame += 1;
                break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                if (displayMenu == MENU_NONE && selectFrame > 1) selectFrame -= 1;
                break;
            default: break;
        }
    }

}



void controllerInput() {
    SDL_GameControllerUpdate();


    Sint16 axisLX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    Sint16 axisLY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    Sint16 axisRX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    // Sint16 axisRY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

    if (abs(axisLY) > 8000) {
        float newX = player.x - cos(player.angle * M_PI / 180) * (axisLY / 32767.0f) * playerMoveSpeed;
        float newY = player.y - sin(player.angle * M_PI / 180) * (axisLY / 32767.0f) * playerMoveSpeed;
        if (colision) {
            if (map[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        } else {
            if (map[(int)newY][(int)newX] != 1) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    if (abs(axisLX) > 8000) {
        float newX = player.x - sin(player.angle * M_PI / 180) * (axisLX / 32767.0f) * playerMoveSpeed / 2;
        float newY = player.y + cos(player.angle * M_PI / 180) * (axisLX / 32767.0f) * playerMoveSpeed / 2;
        if (colision) {
            if (map[(int)newY][(int)newX] == 0) {
                player.x = newX;
                player.y = newY;
            }
        } else {
            if (map[(int)newY][(int)newX] != 1) {
                player.x = newX;
                player.y = newY;
            }
        }
    }

    if (abs(axisRX) > 8000) {
        player.angle += (axisRX / 32767.0f) * playerRotateSpeed;
        if (player.angle < 0) player.angle += 360;
        if (player.angle >= 360) player.angle -= 360;
    }
}