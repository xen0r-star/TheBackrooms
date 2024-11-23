#include "menu.h"
#include "map.h"
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



int initializationMenu() {
    backgroundTexture = IMG_LoadTexture(renderer, BACKGROUND_FILE);

    playButton = (Button){
        {(screenWidth - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    achievementsButton = (Button){
        {(screenWidth - 400) / 2, (275 + 1 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Succès"
    };

    settingsButton = (Button){
        {(screenWidth - 400) / 2, (275 + 2 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Paramètres"
    };

    exitButton = (Button){
        {(screenWidth - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    resumeGameButton = (Button){
        {(screenWidth - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Reprendre"
    };

    extiGameButton = (Button){
        {(screenWidth - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter et sauvegarder"
    };

    loadGame1 = (Button) {
        {50, 50 + 125 * 0 + 20 * 0, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 1"
    };

    loadGame2 = (Button) {
        {50, 50 + 125 * 1 + 20 * 1, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 2"
    };

    loadGame3 = (Button) {
        {50, 50 + 125 * 2 + 20 * 2, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 3"
    };

    launchGame = (Button) {
        {50, 50 + 125 * (4 - 1) + 20 * (4 - 1), screenWidth - 100 - 15, 100}, 
        {0, 0, 0, 255},
        "Start Game"
    };

    return 0;
}



void drawMenu() {
    SDL_SetRelativeMouseMode(SDL_FALSE);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (displayMenu == MENU_MAIN) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        handleButtons(mouseX, mouseY, 4, playButton, achievementsButton, settingsButton, exitButton);
    } else if (displayMenu == MENU_LOAD) { // Jouer
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);

        // for (int i = 0; i < 3; i++) {
        //     SDL_Rect block = {
        //         50, 
        //         50 + 125 * i + 20 * i, 
        //         screenWidth - 100 - 15, 
        //         125
        //     };
        //     SDL_RenderFillRect(renderer, &block);
        // }

        // SDL_Rect block = {
        //     50, 
        //     50 + 125 * (4 - 1) + 20 * (4 - 1), 
        //     screenWidth - 100 - 15, 
        //     100
        // };
        // SDL_RenderFillRect(renderer, &block);

        // Changing condition
        FILE *file;
        file = fopen("./saves/Save1.dat", "r");
        if (file) {
            loadGame1.text = "Charger Sauvegarde 1";
            fclose(file);
        } else {
            loadGame1.text = "Nouvelle Partie";
        }

        file = fopen("./saves/Save2.dat", "r");
        if (file) {
            loadGame2.text = "Charger Sauvegarde 2";
            fclose(file);
        } else {
            loadGame2.text = "Nouvelle Partie";
        }

        file = fopen("./saves/Save3.dat", "r");
        if (file) {
            loadGame3.text = "Charger Sauvegarde 3";
            fclose(file);
        } else {
            loadGame3.text = "Nouvelle Partie";
        }

        drawButton(loadGame1, typeLaunchGame[0]);
        drawButton(loadGame2, typeLaunchGame[1]);
        drawButton(loadGame3, typeLaunchGame[2]);   
        drawButton(launchGame, BUTTON_NORMAL);    

        typeLaunchGame[0] = BUTTON_NORMAL;
        typeLaunchGame[1] = BUTTON_NORMAL;
        typeLaunchGame[2] = BUTTON_NORMAL; 

        generateMap(map, MAP_SIZE, MAP_SIZE, 5);
        for (int y = 0; y < MAP_SIZE; y++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                mapDiscovered[y][x] = 0;
            }
        }

        // displayMenu = 0;
    } else if (displayMenu == MENU_ACHIEVEMENTS) { // Succès
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        int scrollOffset = 0;
        const int successCount = 10;
        const int contentHeight = successCount * 125 + (successCount - 1) * 20;
        const int viewHeight = screenHeight - 50;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
        for (int i = 0; i < successCount; i++) {
            SDL_Rect block = {
                50, 
                50 + 125 * i + 20 * i - scrollOffset, 
                screenWidth - 100 - 15, 
                125
            };
            SDL_RenderFillRect(renderer, &block);
        }

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 240);
        SDL_Rect scrollbar = {screenWidth - 30, 50, 10, viewHeight - 10};
        SDL_RenderFillRect(renderer, &scrollbar);

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        scrollbar = (SDL_Rect){
            screenWidth - 30, 
            50 + ((scrollOffset * viewHeight) / contentHeight), 
            10, 
            (viewHeight * viewHeight) / contentHeight
        };
        SDL_RenderFillRect(renderer, &scrollbar);

    } else if (displayMenu == MENU_SETTINGS) { // Paramètres
        
    } else if (displayMenu == MENU_BREAK) { // Pause
        SDL_UpdateTexture(screenBuffersTexture, NULL, screenBuffers, screenWidth * sizeof(Uint32));
        SDL_RenderCopy(renderer, screenBuffersTexture, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
        SDL_Rect block = {0, 0, screenWidth, screenHeight};
        SDL_RenderFillRect(renderer, &block);

        handleButtons(mouseX, mouseY, 4, resumeGameButton, achievementsButton, settingsButton, extiGameButton);
    }
}