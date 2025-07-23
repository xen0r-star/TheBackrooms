#include "input.h"

#define PI 3.14159265358979323846


void keyboardInput(GameState *state) {
    // Avancer
    if (state->app.keystate[SDL_SCANCODE_UP] || state->app.keystate[26]) {
        float newX = state->playerState.player.x + cos(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed;
        float newY = state->playerState.player.y + sin(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed;
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
        float newX = state->playerState.player.x - cos(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed;
        float newY = state->playerState.player.y - sin(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed;
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
        float newX = state->playerState.player.x + sin(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed / 2;
        float newY = state->playerState.player.y - cos(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed / 2;
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
        float newX = state->playerState.player.x - sin(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed / 2;
        float newY = state->playerState.player.y + cos(state->playerState.player.angle * PI / 180) * state->playerState.playerMoveSpeed / 2;
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