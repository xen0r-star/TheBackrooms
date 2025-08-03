#include "input.h"
#include <math.h>


static int validateControllerState(const GameState *state) {
    if (!state || !state->app.controller) {
        return -1;
    }
    
    return 0;
}

static bool canMoveTo(const GameState *state, float newX, float newY) {
    // Bounds checking
    int mapX = (int)newX;
    int mapY = (int)newY;
    
    if (mapX < 0 || mapX >= state->mapState.mapWidth || 
        mapY < 0 || mapY >= state->mapState.mapHeight) {
        return false;
    }
    
    // Collision checking based on collision mode
    if (state->playerState.collision) {
        // Strict collision: only allow movement to empty spaces
        return (state->mapState.map[mapY][mapX] == 0);
    } else {
        // Relaxed collision: avoid only solid walls (type 1)
        return (state->mapState.map[mapY][mapX] != 1);
    }
}

static void processForwardMovement(GameState *state, Sint16 axisValue) {
    if (abs(axisValue) <= ANALOG_DEADZONE) {
        return;
    }
    
    // Normalize axis value to [-1.0, 1.0]
    float normalizedInput = axisValue / 32767.0f;
    
    // Calculate movement vector (note: negative because forward is negative Y on stick)
    float moveDistance = -normalizedInput * state->playerState.playerMoveSpeed;
    float angleRad = state->playerState.player.angle * PI / 180.0f;
    
    float newX = state->playerState.player.x + cosf(angleRad) * moveDistance;
    float newY = state->playerState.player.y + sinf(angleRad) * moveDistance;
    
    // Apply movement if valid
    if (canMoveTo(state, newX, newY)) {
        state->playerState.player.x = newX;
        state->playerState.player.y = newY;
    }
}

static void processStrafeMovement(GameState *state, Sint16 axisValue) {
    if (abs(axisValue) <= ANALOG_DEADZONE) {
        return;
    }
    
    // Normalize axis value to [-1.0, 1.0]
    float normalizedInput = axisValue / 32767.0f;
    
    // Calculate strafe movement (perpendicular to forward direction)
    float moveDistance = -normalizedInput * state->playerState.playerMoveSpeed * MOVEMENT_DAMPING;
    float angleRad = state->playerState.player.angle * PI / 180.0f;
    
    // Strafe movement is perpendicular to forward direction
    float newX = state->playerState.player.x - sinf(angleRad) * moveDistance;
    float newY = state->playerState.player.y + cosf(angleRad) * moveDistance;
    
    // Apply movement if valid
    if (canMoveTo(state, newX, newY)) {
        state->playerState.player.x = newX;
        state->playerState.player.y = newY;
    }
}

static void processRotation(GameState *state, Sint16 axisValue) {
    if (abs(axisValue) <= ANALOG_DEADZONE) {
        return;
    }
    
    // Normalize axis value and apply rotation
    float normalizedInput = axisValue / 32767.0f;
    float rotationAmount = normalizedInput * state->playerState.playerRotateSpeed;
    
    state->playerState.player.angle += rotationAmount;
    
    // Normalize angle to [0, 360) range
    while (state->playerState.player.angle < 0.0f) {
        state->playerState.player.angle += 360.0f;
    }
    while (state->playerState.player.angle >= 360.0f) {
        state->playerState.player.angle -= 360.0f;
    }
    
    // Update render cache with new direction
    calculateDirRender(state->playerState.player.angle, 
                      &state->graphics.renderCache.dirX, 
                      &state->graphics.renderCache.dirY);
}

void controllerInput(GameState *state) {
    // Validate controller state
    if (validateControllerState(state) != 0) {
        return;
    }
    
    // Update controller state
    SDL_GameControllerUpdate();
    
    // Get analog stick values
    Sint16 leftStickX = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_LEFTX);
    Sint16 leftStickY = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_LEFTY);
    Sint16 rightStickX = SDL_GameControllerGetAxis(state->app.controller, SDL_CONTROLLER_AXIS_RIGHTX);
    
    // Process movement and rotation
    processForwardMovement(state, leftStickY);
    processStrafeMovement(state, leftStickX);
    processRotation(state, rightStickX);
}