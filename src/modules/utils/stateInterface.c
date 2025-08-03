#include "utils.h"
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

void showStateInterface(AppState *appState, const PlayerState playerState, const EntityState entityState)
{
    // Validate input parameters
    if (!appState || !appState->renderer)
    {
        fprintf(stderr, "Error: Invalid app state for debug interface\n");
        return;
    }

    SDL_Renderer *renderer = appState->renderer;
    int screenWidth = appState->screenWidth;
    int screenHeight = appState->screenHeight;

    // Calculate interface dimensions (1/3 screen width, 3/4 screen height for more space)
    int interfaceWidth = screenWidth / 3 - 10;
    int interfaceHeight = (screenHeight * 3) / 4 - 10;

    // Ensure minimum interface size
    if (interfaceWidth < 250)
        interfaceWidth = 250;
    if (interfaceHeight < 300)
        interfaceHeight = 300;

    // Render semi-transparent background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, DEBUG_INTERFACE_OPACITY);
    SDL_Rect backgroundRect = {10, 10, interfaceWidth, interfaceHeight};
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Static variables for FPS display throttling
    static char fpsText[32] = "FPS: --";
    static clock_t lastFPSUpdate = 0;

    // Update FPS text every 500ms to reduce performance impact
    clock_t currentTime = clock();
    if ((currentTime - lastFPSUpdate) * 1000 / CLOCKS_PER_SEC >= FPS_UPDATE_INTERVAL_MS)
    {
        lastFPSUpdate = currentTime;
        snprintf(fpsText, sizeof(fpsText), "FPS: %d", (int)appState->fps);
    }

    // Render FPS display
    renderText(appState, LEFT, 30, 40, fpsText,
               (Color){255, 255, 255, 255}, TEXT_XS);

    // Render separator line
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect separatorRect = {30, 65, interfaceWidth - 50, 1};
    SDL_RenderFillRect(renderer, &separatorRect);

    // Render collision state
    char collisionText[32];
    snprintf(collisionText, sizeof(collisionText), "Collision: %s",
             playerState.collision ? "ON" : "OFF");
    renderText(appState, LEFT, 30, 95, collisionText,
               (Color){255, 255, 255, 255}, TEXT_XS);

    // Render texture display state
    char textureText[32];
    snprintf(textureText, sizeof(textureText), "Textures: %s",
             playerState.showTextures ? "ON" : "OFF");
    renderText(appState, LEFT, 30, 125, textureText,
               (Color){255, 255, 255, 255}, TEXT_XS);

    // Render map display state
    char mapText[32];
    snprintf(mapText, sizeof(mapText), "Map: %s",
             playerState.showMap ? "ON" : "OFF");
    renderText(appState, LEFT, 30, 155, mapText,
               (Color){255, 255, 255, 255}, TEXT_XS);

    // Render second separator line
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect separator2Rect = {30, 180, interfaceWidth - 50, 1};
    SDL_RenderFillRect(renderer, &separator2Rect);

    // Player position information
    char positionText[64];
    snprintf(positionText, sizeof(positionText), "Position: (%.1f, %.1f)",
             playerState.player.x, playerState.player.y);
    renderText(appState, LEFT, 30, 210, positionText,
               (Color){0, 255, 0, 255}, TEXT_XS);

    // Player direction information
    char directionText[64];
    snprintf(directionText, sizeof(directionText), "Direction: %.1f°", playerState.player.angle);
    renderText(appState, LEFT, 30, 240, directionText,
               (Color){0, 255, 0, 255}, TEXT_XS);

    // Movement speeds
    char speedText[64];
    snprintf(speedText, sizeof(speedText), "Speed: Move %.1f | Rot %.1f",
             playerState.playerMoveSpeed, playerState.playerRotateSpeed);
    renderText(appState, LEFT, 30, 270, speedText,
               (Color){0, 255, 0, 255}, TEXT_XS);

    // Render third separator line
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect separator3Rect = {30, 295, interfaceWidth - 50, 1};
    SDL_RenderFillRect(renderer, &separator3Rect);

    // Frame timing information
    static clock_t lastFrameTime = 0;
    static float renderTime = 0.0f;

    clock_t currentFrameTime = clock();
    if (lastFrameTime > 0)
    {
        renderTime = ((float)(currentFrameTime - lastFrameTime) * 1000.0f) / CLOCKS_PER_SEC;
    }
    lastFrameTime = currentFrameTime;

    char renderTimeText[64];
    snprintf(renderTimeText, sizeof(renderTimeText), "Render Time: %.2f ms", renderTime);
    renderText(appState, LEFT, 30, 320, renderTimeText,
               (Color){255, 255, 0, 255}, TEXT_XS);

    // Memory usage (Windows specific)
    char memoryText[64] = "Memory: N/A";
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        float memoryMB = pmc.WorkingSetSize / (1024.0f * 1024.0f);
        snprintf(memoryText, sizeof(memoryText), "Memory: %.1f MB", memoryMB);
    }
#endif
    renderText(appState, LEFT, 30, 350, memoryText,
               (Color){255, 255, 0, 255}, TEXT_XS);

    // Entity count
    char entityText[64];
    snprintf(entityText, sizeof(entityText), "Entities: %d (Max: %d)",
             entityState.numSprites, MAX_SPRITES);
    renderText(appState, LEFT, 30, 380, entityText,
               (Color){255, 255, 0, 255}, TEXT_XS);

    // Render fourth separator line
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect separator4Rect = {30, 405, interfaceWidth - 50, 1};
    SDL_RenderFillRect(renderer, &separator4Rect);

    // Current inventory selection
    char inventoryText[64];
    snprintf(inventoryText, sizeof(inventoryText), "Selected Slot: %d/7",
             playerState.selectFrame);
    renderText(appState, LEFT, 30, 430, inventoryText,
               (Color){0, 255, 255, 255}, TEXT_XS);

    // Game performance metrics
    char performanceText[64];
    float avgFrameTime = (appState->fps > 0) ? (1000.0f / appState->fps) : 0.0f;
    snprintf(performanceText, sizeof(performanceText), "Avg Frame: %.2f ms", avgFrameTime);
    renderText(appState, LEFT, 30, 460, performanceText,
               (Color){0, 255, 255, 255}, TEXT_XS);
}