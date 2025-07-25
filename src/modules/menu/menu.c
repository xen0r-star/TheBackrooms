#include "menu.h"

void drawMenu(GameState *state) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (state->menu.displayMenu == MENU_MAIN) {
        background(state, state->menu.backgroundType);
        renderText(&state->app, CENTER, state->app.screenWidth / 2, 100, "ESCAPE", (Color){255, 255, 255, 255}, TEXT_TITLE);
        renderText(&state->app, CENTER, state->app.screenWidth / 2, 175, "THE BACKROOMS", (Color){255, 255, 255, 255}, TEXT_TITLE);

        handleButtons(&state->app, mouseX, mouseY, 4, 
            state->menu.playButton, 
            state->menu.achievementsButton, 
            state->menu.settingsButton, 
            state->menu.exitButton
        );

    } else if (state->menu.displayMenu == MENU_LOAD) { // Load
        background(state, state->menu.backgroundType);
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

        drawButton(&state->app, state->menu.loadGame1, state->mapState.typeLaunchGame == 1 ? BUTTON_SELECTED : BUTTON_NORMAL);
        drawButton(&state->app, state->menu.loadGame2, state->mapState.typeLaunchGame == 2 ? BUTTON_SELECTED : BUTTON_NORMAL);
        drawButton(&state->app, state->menu.loadGame3, state->mapState.typeLaunchGame == 3 ? BUTTON_SELECTED : BUTTON_NORMAL);   
        drawButton(&state->app, state->menu.launchGame, BUTTON_NORMAL);    

        generateMap(&state->mapState, MAP_SIZE_LEVEL0, MAP_SIZE_LEVEL0);
        spawnPlayerFromMap(&state->playerState, &state->mapState);

        // displayMenu = 0;
    } else if (state->menu.displayMenu == MENU_ACHIEVEMENTS) { // Achievements
        background(state, state->menu.backgroundType);
        
        const int successCount = 10;
        // const int contentHeight = successCount * 125 + (successCount - 1) * 20;
        // const int viewHeight = state->app.screenHeight - 50;


        // Elements
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 225);
        for (int i = 0; i < successCount; i++) {
            SDL_Rect block = {
                60, 
                100 + 125 * i + 20 * i - state->menu.scrollOffset, 
                state->app.screenWidth - 120, 
                125  
            };
            SDL_RenderFillRect(state->app.renderer, &block);
        }


        // Scrollbar
        // SDL_SetRenderDrawColor(state->app.renderer, 200, 200, 200, 240);
        // SDL_Rect scrollbar = {state->app.screenWidth - 30, 50, 10, viewHeight - 10};
        // SDL_RenderFillRect(state->app.renderer, &scrollbar);

        // SDL_SetRenderDrawColor(state->app.renderer, 50, 50, 50, 255);
        // scrollbar = (SDL_Rect){
        //     state->app.screenWidth - 30, 
        //     50 + ((scrollOffset * viewHeight) / contentHeight), 
        //     10, 
        //     (viewHeight * viewHeight) / contentHeight
        // };
        // SDL_RenderFillRect(state->app.renderer, &scrollbar);

        // drawButton(&state->app, state->menu.returnButton, BUTTON_NORMAL);


        // Header
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
        SDL_Rect block = {60, 20, state->app.screenWidth - 120, 50};
        SDL_RenderFillRect(state->app.renderer, &block);

        renderText(&state->app, LEFT, 120, 45, "Succes", (Color){255, 255, 255, 255}, TEXT_S);
        renderText(&state->app, RIGHT, state->app.screenWidth - 60 - 20, 45, "0 / 25", (Color){255, 255, 255, 255}, TEXT_XS);

    } else if (state->menu.displayMenu == MENU_SETTINGS) { // Settings
        background(state, state->menu.backgroundType);
        if (state->menu.backgroundType == BACKGROUND_MENU) {
            
        }

        drawButton(&state->app, state->menu.returnButton, BUTTON_NORMAL);

    } else if (state->menu.displayMenu == MENU_BREAK) { // break
        background(state, state->menu.backgroundType);

        handleButtons(&state->app, mouseX, mouseY, 4, 
            state->menu.resumeGameButton, 
            state->menu.achievementsButton, 
            state->menu.settingsButton, 
            state->menu.exitGameButton
        );
    }
}