#include "handle.h"

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
                if (menu == 0) menu = 5;
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


void mouseMotion(const SDL_Event event) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.xrel;

        player.angle += mouseX * sensitivity;
        if (player.angle < 0) player.angle += 360;
        if (player.angle >= 360) player.angle -= 360;
    } else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0 && selectFrame > 1)     selectFrame -= 1;
        else if (event.wheel.y < 0 && selectFrame < 10) selectFrame += 1;
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
            default:
                break;
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