#include "input.h"

void keyboardDown(GameState *state, const SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_m:
                state->playerState.showMap      = !state->playerState.showMap;
                break;
            case SDLK_f:
                state->playerState.showState    = !state->playerState.showState;
                break;
            case SDLK_t:
                state->playerState.showTextures = !state->playerState.showTextures;
                break;
            case SDLK_c:
                state->playerState.collision    = !state->playerState.collision;
                break;
            case SDLK_ESCAPE:
                if (state->menu.displayMenu == MENU_NONE) state->menu.displayMenu = MENU_BREAK;
                break;
            default:
                break;
        }
    }
}