#include "menu.h"
#include "../data/achievements.h"
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
            // Settings menu (restored to original static state)
            background(state, state->menu.backgroundType);
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

    const int achievementCount = state->achievements.count;
    const int achievementHeight = 125;
    const int achievementSpacing = 20;
    const int sideMargin = 60;

    // Layout constants
    const int headerY = 20;
    const int headerH = 50;
    const int viewportTop = headerY + headerH + 10; // just under header
    const int buttonTop = state->menu.returnButton.rect.y - state->menu.returnButton.rect.h / 2;
    const int viewportBottom = buttonTop - 10;       // just above back button
    int viewportHeight = viewportBottom - viewportTop;
    if (viewportHeight < 10) viewportHeight = 10;    // safety

    // Header bar (background of title section)
    SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
    SDL_Rect headerRect = {sideMargin, headerY, state->app.screenWidth - (sideMargin * 2), headerH};
    SDL_RenderFillRect(state->app.renderer, &headerRect);
    // Bottom divider line for header
    SDL_SetRenderDrawColor(state->app.renderer, 80, 80, 80, 255);
    SDL_RenderDrawLine(state->app.renderer, headerRect.x, headerRect.y + headerRect.h, 
                       headerRect.x + headerRect.w, headerRect.y + headerRect.h);

    // Title and totals
    renderText(&state->app, LEFT, sideMargin + 60, headerY + headerH/2 + 5, 
              "Achievements",
              (Color){255, 255, 255, 255}, TEXT_S);
    int unlocked = 0;
    for (int i=0;i<achievementCount;i++) if (state->achievements.items[i].unlocked) unlocked++;
    char totals[32];
    snprintf(totals, sizeof(totals), "%d / %d", unlocked, achievementCount);
    renderText(&state->app, RIGHT, state->app.screenWidth - sideMargin - 20, headerY + headerH/2 + 5, 
              totals, (Color){255, 255, 255, 255}, TEXT_XS);

    // Compute content and viewport rect; leave room for scrollbar (14px)
    int viewportX = sideMargin;
    int viewportW = state->app.screenWidth - (sideMargin * 2) - 14;
    SDL_Rect clip = {viewportX, viewportTop, viewportW, viewportHeight};

    // Clip rendering to viewport so cards don't draw over header or back button
    SDL_RenderSetClipRect(state->app.renderer, &clip);

    // Background for list area (semi-transparent)
    SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255); // opaque clear to avoid ghosting
    SDL_Rect viewportRect = {viewportX, viewportTop, viewportW, viewportHeight};
    SDL_RenderFillRect(state->app.renderer, &viewportRect);

    // Content height and base position
    int contentHeight = (achievementHeight + achievementSpacing) * achievementCount;
    int baseY = viewportTop;
    int maxOffset = contentHeight > viewportHeight ? (contentHeight - viewportHeight) : 0;
    if (state->menu.scrollOffset < 0) state->menu.scrollOffset = 0;
    if (state->menu.scrollOffset > maxOffset) state->menu.scrollOffset = maxOffset;

    // Compute visible index range to avoid overdraw
    int step = achievementHeight + achievementSpacing;
    int firstIndex = state->menu.scrollOffset / step;
    if (firstIndex > 0) firstIndex -= 1; // render one above for partials
    if (firstIndex < 0) firstIndex = 0;
    int lastIndex = (state->menu.scrollOffset + viewportHeight) / step + 1;
    if (lastIndex >= achievementCount) lastIndex = achievementCount - 1;

    // Render achievement cards within clipped area
    for (int i = firstIndex; i <= lastIndex; i++) {
    int cardY = baseY + (achievementHeight + achievementSpacing) * i - state->menu.scrollOffset;
    int visualCardAdjust = 3; // move card slightly up
    SDL_Rect achievementRect = { viewportX, cardY - visualCardAdjust, viewportW, achievementHeight };

        // Only render if intersects viewport
        if (achievementRect.y + achievementRect.h > viewportTop && achievementRect.y < viewportBottom) {
            // Hover detection
            bool hovered = (mouseX >= achievementRect.x && mouseX <= achievementRect.x + achievementRect.w &&
                            mouseY >= achievementRect.y && mouseY <= achievementRect.y + achievementRect.h);

            // Card shadow
            SDL_Rect shadow = {achievementRect.x + 3, achievementRect.y + 3, achievementRect.w, achievementRect.h};
            SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 90);
            SDL_RenderFillRect(state->app.renderer, &shadow);

            // Card background
            Uint8 baseR = hovered ? 30 : 20;
            Uint8 baseG = hovered ? 30 : 20;
            Uint8 baseB = hovered ? 30 : 20;
            SDL_SetRenderDrawColor(state->app.renderer, baseR, baseG, baseB, 255);
            SDL_RenderFillRect(state->app.renderer, &achievementRect);

            // Accent band at top of card
            SDL_Rect band = {achievementRect.x, achievementRect.y, achievementRect.w, 28};
            Achievement *a = &state->achievements.items[i];
            Uint8 bandR = a->unlocked ? 60 : 100;
            Uint8 bandG = a->unlocked ? 160 : 110;
            Uint8 bandB = a->unlocked ? 60 : 40;
            SDL_SetRenderDrawColor(state->app.renderer, bandR, bandG, bandB, 255);
            SDL_RenderFillRect(state->app.renderer, &band);

            // Card border
            SDL_SetRenderDrawColor(state->app.renderer, hovered ? 180 : 100, hovered ? 180 : 100, hovered ? 180 : 100, 255);
            SDL_RenderDrawRect(state->app.renderer, &achievementRect);

            // Badge placeholder
            SDL_SetRenderDrawColor(state->app.renderer, a->unlocked ? 60 : 30, a->unlocked ? 160 : 30, 60, 255);
            SDL_Rect badge = {achievementRect.x + 15, achievementRect.y + 15, 95, 95};
            SDL_RenderFillRect(state->app.renderer, &badge);
            // Badge border
            SDL_SetRenderDrawColor(state->app.renderer, 200, 200, 200, 255);
            SDL_RenderDrawRect(state->app.renderer, &badge);

            // If locked, draw a simple padlock overlay on badge
            if (!a->unlocked) {
                SDL_SetRenderDrawColor(state->app.renderer, 200, 200, 200, 255);
                SDL_Rect lockBody = {badge.x + 30, badge.y + 48, 36, 28};
                SDL_RenderFillRect(state->app.renderer, &lockBody);
                // Keyhole
                SDL_SetRenderDrawColor(state->app.renderer, 50, 50, 50, 255);
                SDL_Rect keyhole = {lockBody.x + lockBody.w/2 - 3, lockBody.y + 8, 6, 10};
                SDL_RenderFillRect(state->app.renderer, &keyhole);
                // Shackle
                SDL_SetRenderDrawColor(state->app.renderer, 200, 200, 200, 255);
                SDL_Rect shackle = {lockBody.x - 6, lockBody.y - 12, lockBody.w + 12, 10};
                SDL_RenderFillRect(state->app.renderer, &shackle);
            }

            // Title and description (aligned to band and consistent baseline)
            int titleY = band.y + band.h / 2 + 2;                // slightly lower inside band
            int descY  = band.y + band.h + 26;                   // slightly lower baseline below band
            renderText(&state->app, LEFT, achievementRect.x + 130, titleY,
                       a->title, (Color){255,255,255,255}, TEXT_M);
            renderText(&state->app, LEFT, achievementRect.x + 130, descY,
                       a->desc, (Color){200,200,200,255}, TEXT_S);

            // Progress or unlocked label
            char buf[64];
            if (a->unlocked) {
                snprintf(buf, sizeof(buf), "Unlocked");
            } else if (a->target > 0) {
                snprintf(buf, sizeof(buf), "%d / %d", a->progress, a->target);
            } else {
                snprintf(buf, sizeof(buf), "Locked");
            }
            renderText(&state->app, RIGHT, achievementRect.x + achievementRect.w - 20, descY,
                       buf, (Color){255,255,255,255}, TEXT_S);

            // Progress bar
            float ratio = 0.0f;
            if (a->target > 0) ratio = (float)a->progress / (float)a->target;
            else ratio = a->unlocked ? 1.0f : 0.0f;
            if (ratio < 0.0f) {
                ratio = 0.0f;
            }
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            int barX = achievementRect.x + 130;
            int barW = achievementRect.w - 150;
            int barY = achievementRect.y + achievementRect.h - 18;
            int barH = 10;
            // background
            SDL_SetRenderDrawColor(state->app.renderer, 40, 40, 40, 255);
            SDL_Rect barBg = {barX, barY, barW, barH};
            SDL_RenderFillRect(state->app.renderer, &barBg);
            // fill (gradient-ish by lerp red->green)
            int fr = (int)(180 * (1.0f - ratio) + 60 * ratio);
            int fg = (int)(70 * (1.0f - ratio) + 160 * ratio);
            int fb = (int)(60 * (1.0f - ratio) + 60 * ratio);
            SDL_SetRenderDrawColor(state->app.renderer, fr, fg, fb, 255);
            SDL_Rect barFill = {barX, barY, (int)(barW * ratio), barH};
            SDL_RenderFillRect(state->app.renderer, &barFill);
        }
    }

    // Remove clip
    SDL_RenderSetClipRect(state->app.renderer, NULL);

    // Return button (drawn outside clipped area)
    drawButton(&state->app, state->menu.returnButton, BUTTON_NORMAL);

    // Scrollbar in the reserved lane (with track)
    if (contentHeight > viewportHeight) {
        // Track
        int trackX = state->app.screenWidth - sideMargin - 12;
        SDL_SetRenderDrawColor(state->app.renderer, 30, 30, 30, 200);
        SDL_Rect track = {trackX, viewportTop, 10, viewportHeight};
        SDL_RenderFillRect(state->app.renderer, &track);

        float ratio = (float)viewportHeight / (float)contentHeight;
        int barH = (int)(ratio * viewportHeight);
        if (barH < 20) barH = 20;
        float scrollRatio = (maxOffset > 0) ? (float)state->menu.scrollOffset / (float)maxOffset : 0.0f;
        int trackY = viewportTop;
        int trackH = viewportHeight;
        int barY = trackY + (int)(scrollRatio * (trackH - barH));
        int barX = trackX + 2; // inset
        SDL_SetRenderDrawColor(state->app.renderer, 120, 120, 120, 230);
        SDL_Rect bar = {barX, barY, 6, barH};
        SDL_RenderFillRect(state->app.renderer, &bar);
    }
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