#include "menu.h"

bool clickedButton(Button button, int mouseX, int mouseY) {
    return SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &(SDL_Rect){
        button.rect.x - button.rect.w / 2, 
        button.rect.y - button.rect.h / 2, 
        button.rect.w, 
        button.rect.h
    });
}



void drawButton(AppState *appState, Button button, ButtonType type) {
    SDL_Rect rect = {
        button.rect.x - button.rect.w / 2, 
        button.rect.y - button.rect.h / 2, 
        button.rect.w, 
        button.rect.h
    };

    if (type == BUTTON_SELECTED) {
        SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(appState->renderer, &rect);

        rect.x += 5;
        rect.y += 5;
        rect.w -= 10;
        rect.h -= 10;
    }

    SDL_SetRenderDrawColor(appState->renderer, 
        button.color.r, 
        button.color.g, 
        button.color.b, 
        type == BUTTON_HOVER ? 255 : button.color.a
    );
    SDL_RenderFillRect(appState->renderer, &rect);

    renderText(
        appState,
        CENTER,
        button.rect.x, 
        button.rect.y, 
        button.text, 
        (Color){255, 255, 255, 255},
        TEXT_S
    );
}



void handleButtons(AppState *appState, int mouseX, int mouseY, int buttonCount, ...) {
    va_list buttons;
    va_start(buttons, buttonCount);

    bool cursorChanged = false;
    for (int i = 0; i < buttonCount; i++) {
        Button button = va_arg(buttons, Button);

        if (clickedButton(button, mouseX, mouseY)) {
            cursorChanged = true;
            drawButton(appState, button, BUTTON_HOVER);
        } else {
            drawButton(appState, button, BUTTON_NORMAL);
        }
    }

    if (cursorChanged) SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    else               SDL_SetCursor(SDL_GetDefaultCursor());

    va_end(buttons);
}


void handleMenuButtons(GameState *state, int mouseX, int mouseY) {
    int displayMenu = state->menu.displayMenu;

    switch (displayMenu) {
        case MENU_MAIN:
            if (clickedButton(state->menu.playButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_LOAD;
            } else if (clickedButton(state->menu.achievementsButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_ACHIEVEMENTS;
            } else if (clickedButton(state->menu.settingsButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_SETTINGS;
            } else if (clickedButton(state->menu.exitButton, mouseX, mouseY)) {
                state->app.running = false;
            }
            break;
        
        case MENU_LOAD:
            if (clickedButton(state->menu.loadGame1, mouseX, mouseY)) {
                state->mapState.typeLaunchGame = 1;
            } else if (clickedButton(state->menu.loadGame2, mouseX, mouseY)) {
                state->mapState.typeLaunchGame = 2;
            } else if (clickedButton(state->menu.loadGame3, mouseX, mouseY)) {
                state->mapState.typeLaunchGame = 3; 
            } else if (clickedButton(state->menu.launchGame, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_NONE;
                state->menu.backgroundType = BACKGROUND_GAME;
                SDL_SetRelativeMouseMode(SDL_TRUE);
            } else if (clickedButton(state->menu.returnButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_MAIN;
            }
            break;

        case MENU_ACHIEVEMENTS:
            if (clickedButton(state->menu.returnButton, mouseX, mouseY)) {
                state->menu.displayMenu = state->menu.backgroundType == BACKGROUND_GAME ? MENU_BREAK : MENU_MAIN;
            }
            break;
        
        case MENU_SETTINGS:
            if (clickedButton(state->menu.returnButton, mouseX, mouseY)) {
                state->menu.displayMenu = state->menu.backgroundType == BACKGROUND_GAME ? MENU_BREAK : MENU_MAIN;
            }
            break;
        
        case MENU_BREAK:
            if (clickedButton(state->menu.resumeGameButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_NONE;
                SDL_SetRelativeMouseMode(SDL_TRUE);
            } else if (clickedButton(state->menu.achievementsButton, mouseX, mouseY)) { 
                state->menu.displayMenu = MENU_ACHIEVEMENTS;
            } else if (clickedButton(state->menu.settingsButton, mouseX, mouseY)) {     
                state->menu.displayMenu = MENU_SETTINGS;
            } else if (clickedButton(state->menu.exitGameButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_MAIN;
                state->menu.backgroundType = BACKGROUND_MENU;
                saveData(state, "Save1");
            }
            break;
        
        default:
            break;
    }
}