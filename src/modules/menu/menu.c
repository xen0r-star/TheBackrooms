#include "menu.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Forward declarations for internal helper functions
static void updateSaveSlotText(GameState *state);
static void renderAchievementMenu(GameState *state, int mouseX, int mouseY);
static void renderLoadingScreen(GameState *state, const char *message, float progress);


void drawMenu(GameState *state) {
    // Validate input parameters
    if (!state) {
        fprintf(stderr, "Error: Invalid game state for menu rendering\n");
        return;
    }
    
    // Get current mouse position for interaction handling
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    switch (state->menu.displayMenu) {
        case MENU_MAIN:
            // Main menu with animated background
            background(state, state->menu.backgroundType);
            
            // Render main title
            renderText(&state->app, CENTER, state->app.screenWidth / 2, 100, 
                      "ESCAPE", (Color){255, 255, 255, 255}, TEXT_TITLE);
            renderText(&state->app, CENTER, state->app.screenWidth / 2, 175, 
                      "THE BACKROOMS", (Color){255, 255, 255, 255}, TEXT_TITLE);

            // Handle main menu buttons with hover detection
            handleButtons(&state->app, mouseX, mouseY, 4, 
                state->menu.playButton, 
                state->menu.achievementsButton, 
                state->menu.settingsButton, 
                state->menu.exitButton
            );
            break;

        case MENU_LOAD:
            // Save/Load menu with dynamic save slot detection
            background(state, state->menu.backgroundType);
            
            // Apply semi-transparent overlay for readability
            SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 225);

            // Update save slot button text based on save file existence
            updateSaveSlotText(state);
            
            // Render save slot buttons with selection indication
            drawButton(&state->app, state->menu.loadGame1, 
                      (state->mapState.typeLaunchGame == 1) ? BUTTON_SELECTED : BUTTON_NORMAL);
            drawButton(&state->app, state->menu.loadGame2, 
                      (state->mapState.typeLaunchGame == 2) ? BUTTON_SELECTED : BUTTON_NORMAL);
            drawButton(&state->app, state->menu.loadGame3, 
                      (state->mapState.typeLaunchGame == 3) ? BUTTON_SELECTED : BUTTON_NORMAL);   
            drawButton(&state->app, state->menu.launchGame, BUTTON_NORMAL);
            drawButton(&state->app, state->menu.returnButton, BUTTON_NORMAL);

            // Pre-generate map for selected save slot (optimization)
            // generateMap(&state->mapState, MAP_SIZE_LEVEL0, MAP_SIZE_LEVEL0);
            // spawnPlayerFromMap(&state->playerState, &state->mapState);
            break;

        case MENU_ACHIEVEMENTS:
            // Achievement display with scrolling support
            background(state, state->menu.backgroundType);
            
            renderAchievementMenu(state, mouseX, mouseY);
            break;

        case MENU_SETTINGS:
            // Settings menu
            background(state, state->menu.backgroundType);
            
            // TODO: Implement settings interface
            renderText(&state->app, CENTER, state->app.screenWidth / 2, 
                      state->app.screenHeight / 2, "Settings - Coming Soon", 
                      (Color){255, 255, 255, 255}, TEXT_L);
            
            drawButton(&state->app, state->menu.returnButton, BUTTON_NORMAL);
            break;

        case MENU_BREAK:
            // Pause menu with dimmed game background
            background(state, state->menu.backgroundType);

            // Handle pause menu buttons
            handleButtons(&state->app, mouseX, mouseY, 4, 
                state->menu.resumeGameButton, 
                state->menu.achievementsButton, 
                state->menu.settingsButton, 
                state->menu.exitGameButton
            );
            break;
            
        default:
            // Handle unexpected menu states
            fprintf(stderr, "Warning: Unknown menu state %d\n", state->menu.displayMenu);
            break;
    }
}


static void updateSaveSlotText(GameState *state) {
    // Define save file paths
    const char* saveFiles[MAX_SAVE_SLOTS] = {
        "./saves/Save1.dat",
        "./saves/Save2.dat", 
        "./saves/Save3.dat"
    };
    
    // Define button text options
    const char* loadTexts[MAX_SAVE_SLOTS] = {
        "Load Save 1",  // TODO: Localize - was "Charger Sauvegarde 1"
        "Load Save 2",  // TODO: Localize - was "Charger Sauvegarde 2"
        "Load Save 3"   // TODO: Localize - was "Charger Sauvegarde 3"
    };
    
    const char* newGameText = "New Game";  // TODO: Localize - was "Nouvelle Partie"
    
    // Update each save slot button
    Button* saveButtons[MAX_SAVE_SLOTS] = {
        &state->menu.loadGame1,
        &state->menu.loadGame2,
        &state->menu.loadGame3
    };
    
    for (int i = 0; i < MAX_SAVE_SLOTS; i++) {
        FILE *file = fopen(saveFiles[i], "r");
        if (file) {
            saveButtons[i]->text = (char *)loadTexts[i];  // Safe cast for string literals
            fclose(file);
        } else {
            saveButtons[i]->text = (char *)newGameText;   // Safe cast for string literals
        }
    }
}


static void renderAchievementMenu(GameState *state, int mouseX, int mouseY) {
    // Suppress unused parameter warnings for future mouse interaction features
    (void)mouseX;
    (void)mouseY;
    
    const int achievementCount = 10;  // TODO: Load from achievement system
    const int achievementHeight = 125;
    const int achievementSpacing = 20;
    const int sideMargin = 60;
    
    // Render achievement entries
    SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 225);
    for (int i = 0; i < achievementCount; i++) {
        SDL_Rect achievementRect = {
            sideMargin, 
            100 + (achievementHeight + achievementSpacing) * i - state->menu.scrollOffset, 
            state->app.screenWidth - (sideMargin * 2), 
            achievementHeight  
        };
        
        // Only render visible achievements for performance
        if (achievementRect.y + achievementRect.h >= 0 && 
            achievementRect.y < state->app.screenHeight) {
            SDL_RenderFillRect(state->app.renderer, &achievementRect);
            
            // TODO: Render achievement icon, title, description, and progress
        }
    }

    // Render header bar
    SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
    SDL_Rect headerRect = {sideMargin, 20, state->app.screenWidth - (sideMargin * 2), 50};
    SDL_RenderFillRect(state->app.renderer, &headerRect);

    // Header text
    renderText(&state->app, LEFT, sideMargin + 60, 45, 
              "Achievements",  // TODO: Localize - was "Succes"
              (Color){255, 255, 255, 255}, TEXT_S);
    renderText(&state->app, RIGHT, state->app.screenWidth - sideMargin - 20, 45, 
              "0 / 25",  // TODO: Load from achievement system
              (Color){255, 255, 255, 255}, TEXT_XS);
    
    // Return button
    drawButton(&state->app, state->menu.returnButton, BUTTON_NORMAL);
    
    // TODO: Implement scrollbar rendering and mouse wheel handling
}


static void renderLoadingScreen(GameState *state, const char *message, float progress) {
    // Validate parameters
    if (!state || !message) {
        return;
    }
    
    // Clamp progress to valid range
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    
    // Render background
    SDL_Surface *loadingBgSurface = IMG_Load("textures/menu_background.png");
    if (loadingBgSurface) {
        SDL_Texture *loadingBgTexture = SDL_CreateTextureFromSurface(state->app.renderer, loadingBgSurface);
        SDL_FreeSurface(loadingBgSurface);
        
        if (loadingBgTexture) {
            // Render the background image in full screen
            SDL_Rect fullScreen = {0, 0, state->app.screenWidth, state->app.screenHeight};
            SDL_RenderCopy(state->app.renderer, loadingBgTexture, NULL, &fullScreen);
            SDL_DestroyTexture(loadingBgTexture);
        }
    } else {
        // Fallback: black background if image does not load
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
        SDL_Rect overlay = {0, 0, state->app.screenWidth, state->app.screenHeight};
        SDL_RenderFillRect(state->app.renderer, &overlay);
    }
    
    // Calculate centered positions
    int centerX = state->app.screenWidth / 2;
    int centerY = state->app.screenHeight / 2;
    
    // Calculate the size of the text/progress area
    int zoneWidth = 700;   // Width of the area
    int zoneHeight = 350;  // Height of the area
    int zoneX = centerX - zoneWidth / 2;
    int zoneY = centerY - zoneHeight / 2;
    
    // Semi-transparent overlay around content
    SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 180);
    SDL_Rect contentOverlay = {zoneX, zoneY, zoneWidth, zoneHeight};
    SDL_RenderFillRect(state->app.renderer, &contentOverlay);
    
    // Optional border to delimit the area
    SDL_SetRenderDrawColor(state->app.renderer, 100, 100, 100, 200);
    SDL_Rect border = {zoneX - 2, zoneY - 2, zoneWidth + 4, zoneHeight + 4};
    SDL_RenderDrawRect(state->app.renderer, &border);
    
    // Render loading title
    renderText(&state->app, CENTER, centerX, centerY - 100, 
              "LOADING", (Color){255, 255, 255, 255}, TEXT_XL);
    
    // Render status message
    renderText(&state->app, CENTER, centerX, centerY - 50, 
              message, (Color){200, 200, 200, 255}, TEXT_M);
    
    // Progress bar dimensions
    int barWidth = 400;
    int barHeight = 20;
    int barX = centerX - barWidth / 2;
    int barY = centerY;
    
    // Render progress bar background
    SDL_SetRenderDrawColor(state->app.renderer, 50, 50, 50, 255);
    SDL_Rect barBg = {barX - 2, barY - 2, barWidth + 4, barHeight + 4};
    SDL_RenderFillRect(state->app.renderer, &barBg);
    
    // Render progress bar fill with color transition
    int fillWidth = (int)(barWidth * progress);
    if (fillWidth > 0) {
        // Color transitions from yellow to green as progress increases
        int r = (int)(255 * (1.0f - progress * 0.5f));
        int g = 255;
        int b = 0;
        
        SDL_SetRenderDrawColor(state->app.renderer, r, g, b, 255);
        SDL_Rect barFill = {barX, barY, fillWidth, barHeight};
        SDL_RenderFillRect(state->app.renderer, &barFill);
    }
    
    // Render progress percentage
    char progressText[16];
    snprintf(progressText, sizeof(progressText), "%.0f%%", progress * 100.0f);
    renderText(&state->app, CENTER, centerX, centerY + 40, 
              progressText, (Color){255, 255, 255, 255}, TEXT_S);
    
    // Render spinning loading indicator
    static float spinAngle = 0.0f;
    spinAngle += 5.0f; // Rotation speed
    if (spinAngle >= 360.0f) spinAngle -= 360.0f;
    
    // Simple rotating dots indicator
    for (int i = 0; i < 8; i++) {
        float angle = (spinAngle + i * 45.0f) * M_PI / 180.0f;
        int dotX = centerX + (int)(30.0f * cos(angle));
        int dotY = centerY + 110 + (int)(30.0f * sin(angle));
        
        // Fade dots based on position for rotating effect
        int alpha = (int)(255 * (0.3f + 0.7f * (i / 8.0f)));
        
        SDL_SetRenderDrawColor(state->app.renderer, 255, 255, 255, alpha);
        SDL_Rect dot = {dotX - 2, dotY - 2, 4, 4};
        SDL_RenderFillRect(state->app.renderer, &dot);
    }
}

void showLoadingScreen(GameState *state, const char *message, float progress) {
    renderLoadingScreen(state, message, progress);
}