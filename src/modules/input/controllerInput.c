#include "input.h"

#define PI 3.14159265358979323846


void controllerInput(GameState *state) {
    SDL_GameControllerUpdate();


    Sint16 axisLX = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_LEFTX);
    Sint16 axisLY = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_LEFTY);
    Sint16 axisRX = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_RIGHTX);
    // Sint16 axisRY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

    if (abs(axisLY) > 8000) {
        float newX = state->playerState.player.x - cos(state->playerState.player.angle * PI / 180) * (axisLY / 32767.0f) * state->playerState.playerMoveSpeed;
        float newY = state->playerState.player.y - sin(state->playerState.player.angle * PI / 180) * (axisLY / 32767.0f) * state->playerState.playerMoveSpeed;
        
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
        float newX = state->playerState.player.x - sin(state->playerState.player.angle * PI / 180) * (axisLX / 32767.0f) * state->playerState.playerMoveSpeed / 2;
        float newY = state->playerState.player.y + cos(state->playerState.player.angle * PI / 180) * (axisLX / 32767.0f) * state->playerState.playerMoveSpeed / 2;
        
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
        calculateDirRender(state->playerState.player.angle, &state->graphics.renderCache.dirX, &state->graphics.renderCache.dirY);
    }
}