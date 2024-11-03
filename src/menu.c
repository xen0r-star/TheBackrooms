#include "menu.h"
#include "utils.h"



void drawButton(Button button, int type) {
    int x = button.rect.x;
    int y = button.rect.y;
    int w = button.rect.w;
    int h = button.rect.h;

    if (type == BUTTON_FOCUS) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &button.rect);

        x += 5;
        y += 5;
        w -= 10;
        h -= 10;
    }

    SDL_SetRenderDrawColor(renderer, 
        button.color.r, 
        button.color.g, 
        button.color.b, 
        type == BUTTON_SELECTED || type == BUTTON_FOCUS ? 255 : button.color.a
    );
    SDL_RenderFillRect(renderer, &(SDL_Rect){x, y, w, h});

    renderText(
        button.rect.x + (button.rect.w - (int) (strlen(button.text) * 10)) / 2, 
        button.rect.y + (button.rect.h - (int) (button.rect.h * .75)) / 2 + 5, 
        (int) (strlen(button.text) * 10), 
        (int) (button.rect.h * .75) - 10,
        button.text, (SDL_Color){255, 255, 255, 255}
    );
}

bool clickedButton(Button button, int mouseX, int mouseY) {
    return SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button.rect);
}



void handleButtons(int mouseX, int mouseY, int buttonCount, ...) {
    va_list buttons;
    va_start(buttons, buttonCount);

    bool cursorChanged = false;
    for (int i = 0; i < buttonCount; i++) {
        Button button = va_arg(buttons, Button);

        if (clickedButton(button, mouseX, mouseY)) {
            cursorChanged = true;
            drawButton(button, BUTTON_SELECTED);
        } else {
            drawButton(button, BUTTON_NORMAL);
        }
    }

    if (cursorChanged) SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    else               SDL_SetCursor(SDL_GetDefaultCursor());

    va_end(buttons);
}



void menu() {
    SDL_SetRelativeMouseMode(SDL_FALSE);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (displayMenu == 1) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        handleButtons(mouseX, mouseY, 4, playButton, achievementsButton, settingsButton, exitButton);
    } else if (displayMenu == 2) { // Jouer
        displayMenu = 0;
        
    } else if (displayMenu == 3) { // Succès
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        int scrollOffset = 0;
        const int successCount = 10;
        const int contentHeight = successCount * 125 + (successCount - 1) * 20;
        const int viewHeight = SCREEN_HEIGHT - 50;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
        for (int i = 0; i < successCount; i++) {
            SDL_Rect block = {
                50, 
                50 + 125 * i + 20 * i - scrollOffset, 
                SCREEN_WIDTH - 100 - 15, 
                125
            };
            SDL_RenderFillRect(renderer, &block);
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 240);
        SDL_Rect scrollbar = {SCREEN_WIDTH - 30, 50, 10, viewHeight - 10};
        SDL_RenderFillRect(renderer, &scrollbar);

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        scrollbar = (SDL_Rect){
            SCREEN_WIDTH - 30, 
            50 + ((scrollOffset * viewHeight) / contentHeight), 
            10, 
            (viewHeight * viewHeight) / contentHeight
        };
        SDL_RenderFillRect(renderer, &scrollbar);

    } else if (displayMenu == 4) { // Paramètres
        
    } else if (displayMenu == 5) { // Pause
        SDL_UpdateTexture(screenBuffersTexture, NULL, screenBuffers, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer, screenBuffersTexture, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
        SDL_Rect block = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &block);

        handleButtons(mouseX, mouseY, 4, resumeGameButton, achievementsButton, settingsButton, extiGameButton);
    }
}