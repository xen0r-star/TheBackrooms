#include "menu.h"
#include "map.h"
#include "utils.h"


int initializationMenu(GameState *state) {
    state->menu.backgroundTexture = IMG_LoadTexture(state->app.renderer, BACKGROUND_FILE);
    
    int screenWidth = state->app.screenWidth;

    state->menu.playButton = (Button){
        {(screenWidth - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    state->menu.achievementsButton = (Button){
        {(screenWidth - 400) / 2, (275 + 1 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Succès"
    };

    state->menu.settingsButton = (Button){
        {(screenWidth - 400) / 2, (275 + 2 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Paramètres"
    };

    state->menu.exitButton = (Button){
        {(screenWidth - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    state->menu.resumeGameButton = (Button){
        {(screenWidth - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Reprendre"
    };

    state->menu.exitGameButton = (Button){
        {(screenWidth - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter et sauvegarder"
    };

    state->menu.loadGame1 = (Button) {
        {50, 50 + 125 * 0 + 20 * 0, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 1"
    };

    state->menu.loadGame2 = (Button) {
        {50, 50 + 125 * 1 + 20 * 1, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 2"
    };

    state->menu.loadGame3 = (Button) {
        {50, 50 + 125 * 2 + 20 * 2, screenWidth - 100 - 15, 125}, 
        {0, 0, 0, 255},
        "Load Game 3"
    };

    state->menu.launchGame = (Button) {
        {50, 50 + 125 * (4 - 1) + 20 * (4 - 1), screenWidth - 100 - 15, 100}, 
        {0, 0, 0, 255},
        "Start Game"
    };

    return 0;
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

bool clickedButton(Button button, int mouseX, int mouseY) {
    return SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button.rect);
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



void drawMenu(GameState *state) {
    SDL_SetRelativeMouseMode(SDL_FALSE);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (state->menu.displayMenu == MENU_MAIN) {
        SDL_RenderCopy(state->app.renderer, state->menu.backgroundTexture, NULL, NULL);
        handleButtons(&state->app, mouseX, mouseY, 4, 
            state->menu.playButton, 
            state->menu.achievementsButton, 
            state->menu.settingsButton, 
            state->menu.exitButton
        );
    } else if (state->menu.displayMenu == MENU_LOAD) { // Jouer
        SDL_RenderCopy(state->app.renderer, state->menu.backgroundTexture, NULL, NULL);
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 225);

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
            state->menu.loadGame1.text = "Charger Sauvegarde 1";
            fclose(file);
        } else {
            state->menu.loadGame1.text = "Nouvelle Partie";
        }

        file = fopen("./saves/Save2.dat", "r");
        if (file) {
            state->menu.loadGame2.text = "Charger Sauvegarde 2";
            fclose(file);
        } else {
            state->menu.loadGame2.text = "Nouvelle Partie";
        }

        file = fopen("./saves/Save3.dat", "r");
        if (file) {
            state->menu.loadGame3.text = "Charger Sauvegarde 3";
            fclose(file);
        } else {
            state->menu.loadGame3.text = "Nouvelle Partie";
        }

        drawButton(&state->app, state->menu.loadGame1, state->mapState.typeLaunchGame[0]);
        drawButton(&state->app, state->menu.loadGame2, state->mapState.typeLaunchGame[1]);
        drawButton(&state->app, state->menu.loadGame3, state->mapState.typeLaunchGame[2]);   
        drawButton(&state->app, state->menu.launchGame, BUTTON_NORMAL);    

        state->mapState.typeLaunchGame[0] = BUTTON_NORMAL;
        state->mapState.typeLaunchGame[1] = BUTTON_NORMAL;
        state->mapState.typeLaunchGame[2] = BUTTON_NORMAL; 

        generateMap(state->mapState.map, MAP_SIZE, MAP_SIZE, 5);
        for (int y = 0; y < MAP_SIZE; y++) {
            for (int x = 0; x < MAP_SIZE; x++) {
                state->mapState.mapDiscovered[y][x] = 0;
            }
        }

        // displayMenu = 0;
    } else if (state->menu.displayMenu == MENU_ACHIEVEMENTS) { // Succès
        SDL_RenderCopy(state->app.renderer, state->menu.backgroundTexture, NULL, NULL);

        int scrollOffset = 0;
        const int successCount = 10;
        const int contentHeight = successCount * 125 + (successCount - 1) * 20;
        const int viewHeight = state->app.screenHeight - 50;


        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 225);
        for (int i = 0; i < successCount; i++) {
            SDL_Rect block = {
                50, 
                50 + 125 * i + 20 * i - scrollOffset, 
                state->app.screenWidth - 100 - 15, 
                125
            };
            SDL_RenderFillRect(state->app.renderer, &block);
        }

        SDL_SetRenderDrawColor(state->app.renderer, 200, 200, 200, 240);
        SDL_Rect scrollbar = {state->app.screenWidth - 30, 50, 10, viewHeight - 10};
        SDL_RenderFillRect(state->app.renderer, &scrollbar);

        SDL_SetRenderDrawColor(state->app.renderer, 50, 50, 50, 255);
        scrollbar = (SDL_Rect){
            state->app.screenWidth - 30, 
            50 + ((scrollOffset * viewHeight) / contentHeight), 
            10, 
            (viewHeight * viewHeight) / contentHeight
        };
        SDL_RenderFillRect(state->app.renderer, &scrollbar);

    } else if (state->menu.displayMenu == MENU_SETTINGS) { // Paramètres
        
    } else if (state->menu.displayMenu == MENU_BREAK) { // Pause
        SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, 
            state->graphics.screenBuffers, 
            state->app.screenWidth * sizeof(Uint32)
        );
        SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);

        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 190);
        SDL_Rect block = {0, 0, state->app.screenWidth, state->app.screenHeight};
        SDL_RenderFillRect(state->app.renderer, &block);

        handleButtons(&state->app, mouseX, mouseY, 4, 
            state->menu.resumeGameButton, 
            state->menu.achievementsButton, 
            state->menu.settingsButton, 
            state->menu.exitGameButton
        );
    }
}