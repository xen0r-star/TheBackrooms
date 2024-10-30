#include "keyboard.h"

void keyboardDown(const SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_m:
                showMap = !showMap;
                if (!showMap) showRays = false;
                break;
            case SDLK_r:
                if(showMap) showRays = !showRays;
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



void keyboardInput(const Uint8 *keystate) {
    // Avancer
    if (keystate[SDL_SCANCODE_UP] || keystate[26]) {
        float newX = player.x + cos(player.angle * M_PI / 180) * moveSpeed;
        float newY = player.y + sin(player.angle * M_PI / 180) * moveSpeed;
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
        float newX = player.x - cos(player.angle * M_PI / 180) * moveSpeed;
        float newY = player.y - sin(player.angle * M_PI / 180) * moveSpeed;
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

    // Tourner à gauche
    if (keystate[SDL_SCANCODE_LEFT] || keystate[4]) {
        player.angle -= rotateSpeed;
        if (player.angle < 0) player.angle += 360;
    }

    // Tourner à droite
    if (keystate[SDL_SCANCODE_RIGHT] || keystate[7]) {
        player.angle += rotateSpeed;
        if (player.angle >= 360) player.angle -= 360;
    }
}