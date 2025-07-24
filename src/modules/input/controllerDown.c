#include "input.h"

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
                if (state->menu.displayMenu == MENU_NONE && state->playerState.selectFrame < 7) state->playerState.selectFrame += 1;
                break;
            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                if (state->menu.displayMenu == MENU_NONE && state->playerState.selectFrame > 1)  state->playerState.selectFrame -= 1;
                break;
            default: break;
        }
    }

}