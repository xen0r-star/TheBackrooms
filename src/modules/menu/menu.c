#include "menu.h"

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
    } else if (state->menu.displayMenu == MENU_LOAD) { // Load
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
    } else if (state->menu.displayMenu == MENU_ACHIEVEMENTS) { // Achievements
        SDL_RenderCopy(state->app.renderer, state->menu.backgroundTexture, NULL, NULL);

        int scrollOffset = 0;
        const int successCount = 10;
        const int contentHeight = successCount * 125 + (successCount - 1) * 20;
        const int viewHeight = state->app.screenHeight - 50;

        // Elements
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

    } else if (state->menu.displayMenu == MENU_SETTINGS) { // Settings
        
    } else if (state->menu.displayMenu == MENU_BREAK) { // break
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