#include "menu.h"


bool clickedButton(Button button, int mouseX, int mouseY) {
    // Calculate button rectangle from center-based coordinates
    SDL_Rect buttonRect = {
        button.rect.x - button.rect.w / 2, 
        button.rect.y - button.rect.h / 2, 
        button.rect.w, 
        button.rect.h
    };
    
    SDL_Point mousePoint = {mouseX, mouseY};
    return SDL_PointInRect(&mousePoint, &buttonRect);
}


void drawButton(AppState *appState, Button button, ButtonType type) {
    // Validate input parameters
    if (!appState || !appState->renderer) {
        fprintf(stderr, "Error: Invalid app state for button rendering\n");
        return;
    }
    
    // Calculate button rectangle from center coordinates
    SDL_Rect rect = {
        button.rect.x - button.rect.w / 2, 
        button.rect.y - button.rect.h / 2, 
        button.rect.w, 
        button.rect.h
    };

    // Render selection border for selected buttons
    if (type == BUTTON_SELECTED) {
        SDL_SetRenderDrawColor(appState->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(appState->renderer, &rect);

        // Shrink inner rectangle to create border effect
        const int borderWidth = 5;
        rect.x += borderWidth;
        rect.y += borderWidth;
        rect.w -= borderWidth * 2;
        rect.h -= borderWidth * 2;
    }

    // Calculate button color with hover effect
    Uint8 alpha = (type == BUTTON_HOVER) ? 255 : button.color.a;
    
    SDL_SetRenderDrawColor(appState->renderer, 
        button.color.r, 
        button.color.g, 
        button.color.b, 
        alpha
    );
    SDL_RenderFillRect(appState->renderer, &rect);

    // Render centered button text
    renderText(
        appState,
        CENTER,
        button.rect.x, 
        button.rect.y, 
        button.text, 
        (Color){255, 255, 255, 255},  // White text for contrast
        TEXT_S
    );
}


void handleButtons(AppState *appState, int mouseX, int mouseY, int buttonCount, ...) {
    // Validate input parameters
    if (!appState || buttonCount <= 0) {
        fprintf(stderr, "Error: Invalid parameters for button handling\n");
        return;
    }
    
    va_list buttons;
    va_start(buttons, buttonCount);

    bool cursorChanged = false;
    
    // Process each button in the list
    for (int i = 0; i < buttonCount; i++) {
        Button button = va_arg(buttons, Button);

        // Check for hover state and render accordingly
        if (clickedButton(button, mouseX, mouseY)) {
            cursorChanged = true;
            drawButton(appState, button, BUTTON_HOVER);
        } else {
            drawButton(appState, button, BUTTON_NORMAL);
        }
    }

    // Update cursor state based on hover detection
    if (cursorChanged) {
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    } else {
        SDL_SetCursor(SDL_GetDefaultCursor());
    }

    va_end(buttons);
}


void handleMenuButtons(GameState *state, int mouseX, int mouseY) {
    // Validate input parameters
    if (!state) {
        fprintf(stderr, "Error: Invalid game state for menu button handling\n");
        return;
    }
    
    int displayMenu = state->menu.displayMenu;

    switch (displayMenu) {
        case MENU_MAIN:
            // Main menu navigation
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
            // Save slot selection and game launch
            if (clickedButton(state->menu.loadGame1, mouseX, mouseY)) {
                state->mapState.typeLaunchGame = 1;
            } else if (clickedButton(state->menu.loadGame2, mouseX, mouseY)) {
                state->mapState.typeLaunchGame = 2;
            } else if (clickedButton(state->menu.loadGame3, mouseX, mouseY)) {
                state->mapState.typeLaunchGame = 3; 
            } else if (clickedButton(state->menu.launchGame, mouseX, mouseY)) {
                // Check if save file exists for selected slot
                char saveFileName[64];
                snprintf(saveFileName, sizeof(saveFileName), "saves/Save%d.dat", state->mapState.typeLaunchGame);
                FILE *saveFile = fopen(saveFileName, "r");
                
                if (saveFile != NULL) {
                    // Load existing save file
                    fclose(saveFile);
                    
                    showLoadingScreen(state, "Loading saved game...", 0.0f);
                    SDL_RenderPresent(state->app.renderer);
                    SDL_Delay(100);
                    
                    showLoadingScreen(state, "Reading save data...", 0.4f);
                    SDL_RenderPresent(state->app.renderer);
                    
                    // Create save name without path and extension
                    char saveName[32];
                    snprintf(saveName, sizeof(saveName), "Save%d", state->mapState.typeLaunchGame);
                    readData(state, saveName);
                    SDL_Delay(150);
                    
                    showLoadingScreen(state, "Initializing world...", 0.8f);
                    SDL_RenderPresent(state->app.renderer);
                    SDL_Delay(100);
                    
                    showLoadingScreen(state, "Game loaded successfully!", 1.0f);
                    SDL_RenderPresent(state->app.renderer);
                    SDL_Delay(250);
                } else {
                    // Create new game with loading sequence
                    showLoadingScreen(state, "Creating new world...", 0.0f);
                    SDL_RenderPresent(state->app.renderer);
                    SDL_Delay(150);
                    
                    showLoadingScreen(state, "Generating maze layout...", 0.3f);
                    SDL_RenderPresent(state->app.renderer);
                    generateMap(&state->mapState, MAP_SIZE_LEVEL0, MAP_SIZE_LEVEL0);
                    SDL_Delay(200);
                    
                    showLoadingScreen(state, "Placing entities...", 0.6f);
                    SDL_RenderPresent(state->app.renderer);
                    spawnPlayerFromMap(&state->playerState, &state->mapState);
                    SDL_Delay(150);
                    
                    showLoadingScreen(state, "Finalizing world...", 0.9f);
                    SDL_RenderPresent(state->app.renderer);
                    SDL_Delay(100);
                    
                    showLoadingScreen(state, "Welcome to The Backrooms!", 1.0f);
                    SDL_RenderPresent(state->app.renderer);
                    SDL_Delay(400);
                }
                
                // Start game with selected save slot
                state->menu.displayMenu = MENU_NONE;
                state->menu.backgroundType = BACKGROUND_GAME;
                SDL_SetRelativeMouseMode(SDL_TRUE);  // Capture mouse for game
            } else if (clickedButton(state->menu.returnButton, mouseX, mouseY)) {
                state->menu.displayMenu = MENU_MAIN;
            }
            break;

        case MENU_ACHIEVEMENTS:
            // Achievement menu navigation
            if (clickedButton(state->menu.returnButton, mouseX, mouseY)) {
                // Return to appropriate parent menu based on context
                state->menu.displayMenu = (state->menu.backgroundType == BACKGROUND_GAME) 
                    ? MENU_BREAK : MENU_MAIN;
            }
            break;
        
        case MENU_SETTINGS:
            // Settings menu navigation
            if (clickedButton(state->menu.returnButton, mouseX, mouseY)) {
                // Return to appropriate parent menu based on context
                state->menu.displayMenu = (state->menu.backgroundType == BACKGROUND_GAME) 
                    ? MENU_BREAK : MENU_MAIN;
            }
            break;
        
        case MENU_BREAK:
            // Pause menu navigation
            if (clickedButton(state->menu.resumeGameButton, mouseX, mouseY)) {
                // Resume game and capture mouse
                state->menu.displayMenu = MENU_NONE;
                SDL_SetRelativeMouseMode(SDL_TRUE);
            } else if (clickedButton(state->menu.achievementsButton, mouseX, mouseY)) { 
                state->menu.displayMenu = MENU_ACHIEVEMENTS;
            } else if (clickedButton(state->menu.settingsButton, mouseX, mouseY)) {     
                state->menu.displayMenu = MENU_SETTINGS;
            } else if (clickedButton(state->menu.exitGameButton, mouseX, mouseY)) {
                // Exit to main menu and save progress
                showLoadingScreen(state, "Saving game progress...", 0.0f);
                SDL_RenderPresent(state->app.renderer);
                SDL_Delay(200);
                
                showLoadingScreen(state, "Writing save data...", 0.5f);
                SDL_RenderPresent(state->app.renderer);
                saveData(state, "Save1");  // Auto-save current progress
                SDL_Delay(300);
                
                showLoadingScreen(state, "Game saved successfully!", 1.0f);
                SDL_RenderPresent(state->app.renderer);
                SDL_Delay(400);
                
                state->menu.displayMenu = MENU_MAIN;
                state->menu.backgroundType = BACKGROUND_MENU;
            }
            break;
        
        default:
            // Handle unexpected menu states
            fprintf(stderr, "Warning: Unknown menu state %d\n", displayMenu);
            break;
    }
}