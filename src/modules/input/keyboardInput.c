#include "input.h"
#include <math.h>


static int validateKeyboardState(const GameState *state) {
    if (!state || !state->app.keystate) {
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

static void processMovement(GameState *state, MovementDirection direction) {
    float angleRad = state->playerState.player.angle * PI / 180.0f;
    float moveSpeed = state->playerState.playerMoveSpeed;
    float newX = state->playerState.player.x;
    float newY = state->playerState.player.y;
    
    switch (direction) {
        case MOVE_FORWARD:
            // Move forward (positive direction)
            newX += cosf(angleRad) * moveSpeed;
            newY += sinf(angleRad) * moveSpeed;
            break;
            
        case MOVE_BACKWARD:
            // Move backward (negative direction)
            newX -= cosf(angleRad) * moveSpeed;
            newY -= sinf(angleRad) * moveSpeed;
            break;
            
        case MOVE_STRAFE_LEFT:
            // Strafe left (perpendicular to forward, reduced speed)
            newX += sinf(angleRad) * moveSpeed * MOVEMENT_DAMPING;
            newY -= cosf(angleRad) * moveSpeed * MOVEMENT_DAMPING;
            break;
            
        case MOVE_STRAFE_RIGHT:
            // Strafe right (perpendicular to forward, reduced speed)
            newX -= sinf(angleRad) * moveSpeed * MOVEMENT_DAMPING;
            newY += cosf(angleRad) * moveSpeed * MOVEMENT_DAMPING;
            break;
    }
    
    // Apply movement if valid
    if (canMoveTo(state, newX, newY)) {
        state->playerState.player.x = newX;
        state->playerState.player.y = newY;
    }
}

static void processRotation(GameState *state, float rotationDirection) {
    state->playerState.player.angle += rotationDirection * state->playerState.playerRotateSpeed;
    
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

void keyboardInput(GameState *state) {
    // Validate keyboard state
    if (validateKeyboardState(state) != 0) {
        return;
    }
    
    const Uint8 *keystate = state->app.keystate;
    
    // Movement input processing (WASD + Arrow keys)
    
    // Forward movement (W key or Up arrow)
    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        processMovement(state, MOVE_FORWARD);
    }
    
    // Backward movement (S key or Down arrow)
    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        processMovement(state, MOVE_BACKWARD);
    }
    
    // Strafe left movement (A key)
    if (keystate[SDL_SCANCODE_A]) {
        processMovement(state, MOVE_STRAFE_LEFT);
    }
    
    // Strafe right movement (D key)
    if (keystate[SDL_SCANCODE_D]) {
        processMovement(state, MOVE_STRAFE_RIGHT);
    }
    
    // Rotation input processing
    
    // Rotate left (Left arrow or Q key)
    if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_Q]) {
        processRotation(state, -1.0f);
    }
    
    // Rotate right (Right arrow or E key)
    if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_E]) {
        processRotation(state, 1.0f);
    }
}