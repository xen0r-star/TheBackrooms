#include "input.h"


static int validateControllerEvent(const GameState *state, const SDL_Event event) {
    if (!state) {
        return -1;
    }
    
    if (event.type != SDL_CONTROLLERBUTTONDOWN) {
        return -1;
    }
    
    return 0;
}

static void adjustFrameSelection(GameState *state, int increment) {
    const int MIN_FRAME = 1;
    const int MAX_FRAME = 7;
    
    if (state->menu.displayMenu != MENU_NONE) {
        return; // Only allow frame selection during gameplay
    }
    
    int newFrame = state->playerState.selectFrame + increment;
    
    if (newFrame >= MIN_FRAME && newFrame <= MAX_FRAME) {
        state->playerState.selectFrame = newFrame;
    }
}

void controllerDown(GameState *state, const SDL_Event event) {
    // Validate input parameters
    if (validateControllerEvent(state, event) != 0) {
        return;
    }

    // Process controller button press events
    switch (event.cbutton.button) {
        case SDL_CONTROLLER_BUTTON_Y:
            // Toggle minimap display
            state->playerState.showMap = !state->playerState.showMap;
            break;
            
        case SDL_CONTROLLER_BUTTON_BACK:
            // Toggle texture rendering mode
            state->playerState.showTextures = !state->playerState.showTextures;
            break;
            
        case SDL_CONTROLLER_BUTTON_START:
            // Toggle debug state display
            state->playerState.showState = !state->playerState.showState;
            break;
            
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            // Cycle frame selection forward
            adjustFrameSelection(state, 1);
            break;
            
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            // Cycle frame selection backward
            adjustFrameSelection(state, -1);
            break;
            
        case SDL_CONTROLLER_BUTTON_A:
            // Add interaction button for future use
            // Currently reserved for future interaction system
            break;
            
        case SDL_CONTROLLER_BUTTON_B:
            // Add cancel/back button for future menu navigation
            // Currently reserved for menu system
            break;
            
        default:
            // Unhandled button - no action needed
            break;
    }
}