#include "menu.h"

bool clickedButton(Button button, int mouseX, int mouseY) {
    return SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button.rect);
}



void drawButton(AppState *appState, Button button, ButtonType type) {
    int x = button.rect.x;
    int y = button.rect.y;
    int w = button.rect.w;
    int h = button.rect.h;

    if (type == BUTTON_FOCUS) {
        SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(appState->renderer, &button.rect);

        x += 5;
        y += 5;
        w -= 10;
        h -= 10;
    }

    SDL_SetRenderDrawColor(appState->renderer, 
        button.color.r, 
        button.color.g, 
        button.color.b, 
        type == BUTTON_SELECTED || type == BUTTON_FOCUS ? 255 : button.color.a
    );
    SDL_RenderFillRect(appState->renderer, &(SDL_Rect){x, y, w, h});

    renderText(
        appState,
        button.rect.x + (button.rect.w - (int) (strlen(button.text) * 10)) / 2, 
        button.rect.y + (button.rect.h - (int) (button.rect.h * .75)) / 2 + 5, 
        (int) (strlen(button.text) * 10), 
        (int) (button.rect.h * .75) - 10,
        button.text, (SDL_Color){255, 255, 255, 255}
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
            drawButton(appState, button, BUTTON_SELECTED);
        } else {
            drawButton(appState, button, BUTTON_NORMAL);
        }
    }

    if (cursorChanged) SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    else               SDL_SetCursor(SDL_GetDefaultCursor());

    va_end(buttons);
}