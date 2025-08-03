#include "input.h"


static int validateKeyboardEvent(const GameState *state, const SDL_Event event) {
    if (!state) {
        return -1;
    }
    
    if (event.type != SDL_KEYDOWN) {
        return -1;
    }
    
    return 0;
}

static void handleDebugCommands(GameState *state, SDL_Keycode key) {
    switch (key) {
        case SDLK_m:
            // Toggle minimap display
            state->playerState.showMap = !state->playerState.showMap;
            break;
            
        case SDLK_f:
            // Toggle debug state display
            state->playerState.showState = !state->playerState.showState;
            break;
            
        case SDLK_t:
            // Toggle texture rendering mode
            state->playerState.showTextures = !state->playerState.showTextures;
            break;
            
        case SDLK_c:
            // Toggle collision detection
            state->playerState.collision = !state->playerState.collision;
            break;
            
        default:
            // Key not handled by debug commands
            break;
    }
}

static void handleSystemCommands(GameState *state, SDL_Keycode key) {
    switch (key) {
        case SDLK_ESCAPE:
            // Open pause menu if in game, close menu if already open
            if (state->menu.displayMenu == MENU_NONE) {
                state->menu.displayMenu = MENU_BREAK;
                SDL_SetRelativeMouseMode(SDL_FALSE); // Enable cursor for menu
            }
            break;
            
        case SDLK_F1:
            // Help/Controls (reserved for future implementation)
            break;
            
        case SDLK_F11:
            // Fullscreen toggle (reserved for future implementation)
            break;
            
        default:
            // Key not handled by system commands
            break;
    }
}

void keyboardDown(GameState *state, const SDL_Event event) {
    // Validate input parameters
    if (validateKeyboardEvent(state, event) != 0) {
        return;
    }
    
    SDL_Keycode key = event.key.keysym.sym;
    
    // Handle different categories of key commands
    handleDebugCommands(state, key);
    handleSystemCommands(state, key);
}