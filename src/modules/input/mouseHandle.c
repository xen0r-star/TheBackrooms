#include "input.h"

void mouseHandle(GameState *state, const SDL_Event event) {
    int displayMenu = state->menu.displayMenu;

    if (event.type == SDL_MOUSEMOTION && displayMenu == MENU_NONE) {
        int mouseX = event.motion.xrel;

        state->playerState.player.angle += mouseX * state->settings.sensitivity;
        if (state->playerState.player.angle < 0)    state->playerState.player.angle += 360;
        if (state->playerState.player.angle >= 360) state->playerState.player.angle -= 360;

    } else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0 && state->playerState.selectFrame > 1)       state->playerState.selectFrame -= 1;
        else if (event.wheel.y < 0 && state->playerState.selectFrame < 5)  state->playerState.selectFrame += 1;

        if (event.wheel.y > 0 && state->menu.scrollOffset > 0) {
            state->menu.scrollOffset = (state->menu.scrollOffset - 20 < 0) ? 0 : state->menu.scrollOffset - 20;
        } else if (event.wheel.y < 0) {
            int maxScrollOffset = (10 - 3) * 125 + ((10 - 3) - 1) * 20;
            state->menu.scrollOffset = (state->menu.scrollOffset + 20 > maxScrollOffset) ? maxScrollOffset : state->menu.scrollOffset + 20;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && displayMenu != MENU_NONE) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        handleMenuButtons(state, mouseX, mouseY);
    }
}