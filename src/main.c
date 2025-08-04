#include "graphics/graphics.h"
#include "input/input.h"
#include "menu/menu.h"
#include "utils/utils.h"
#include "core/core.h"
#include "data/data.h"
#include "entities/entity.h"


GameState gameState = {
    .app = {800, 600, true, NULL, NULL, NULL, NULL, {NULL}, 0, 0, 0},
    .playerState = {{1.0, 1.0, 0.0}, 150.0, 3.0, 0.0, 0.0, 1, false, false, true, true},
    .entityState = {NULL, 0, NULL}, 
    .menu = {MENU_MAIN, BACKGROUND_MENU, 0},
    .mapState = {MAP_SIZE_LEVEL0, MAP_SIZE_LEVEL0, NULL, NULL, 0},
    .graphics = {NULL, NULL, NULL, {0}},
    .settings = {0.2, 85.0, 1.0}
};


int main(int argc, char *argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    // Allocate and initialize game state
    GameState *state = malloc(sizeof(GameState));
    if (!state) {
        fprintf(stderr, "Error: Failed to allocate memory for game state\n");
        return 1;
    }
    *state = gameState;

    // Initialize all game subsystems with error checking
    if (initializationWindow(state)) {
        fprintf(stderr, "Error: Failed to initialize window\n");
        free(state);
        return 1;
    }
    
    if (initializationMenu(state)) {
        fprintf(stderr, "Error: Failed to initialize menu system\n");
        closeWindow(state);
        return 1;
    }
    
    if (initializationScreen(state)) {
        fprintf(stderr, "Error: Failed to initialize screen buffers\n");
        closeWindow(state);
        return 1;
    }
    
    if (initializationTextures(state)) {
        fprintf(stderr, "Error: Failed to load textures\n");
        closeWindow(state);
        return 1;
    }

    // Initialize test entity with bread texture
    initializationEntity(&state->entityState, (Sprite){
        .x = 4.0,
        .y = 4.0,
        .scaleX = 0.8f,
        .scaleY = 0.8f,
        .texture_id = 4,
        .transform = {.transparency = 1.0f, .moveY = -64.0f}
    });

    // Initialize timing for FPS calculation
    state->app.startTime = clock();
    state->app.previousTime = clock();

    // Main game loop
    SDL_Event event;
    while (state->app.running) {
        // Process all pending events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                state->app.running = false;
                break;
            }

            // Handle input events
            mouseHandle(state, event);
            keyboardDown(state, event);

            // Handle controller input if available
            if (state->app.controller != NULL) {
                controllerDown(state, event);
            }
        }

        // Handle window resize events
        if (hasWindowResize(&state->app)) {
            printf("Window resize detected, reinitializing graphics\n");
            
            // Clean up existing graphics resources
            SDL_DestroyTexture(state->graphics.screenBuffersTexture);
            free(state->graphics.screenBuffers);
            state->graphics.screenBuffers = NULL;
            state->graphics.screenBuffersTexture = NULL;

            // Update window dimensions
            SDL_GetWindowSize(state->app.window, &state->app.screenWidth, &state->app.screenHeight);

            // Reinitialize graphics systems
            if (initializationScreen(state)) {
                fprintf(stderr, "Error: Failed to reinitialize screen after resize\n");
                break;
            }
            if (initializationMenu(state)) {
                fprintf(stderr, "Error: Failed to reinitialize menu after resize\n");
                break;
            }
        }

        // Clear screen with black background
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state->app.renderer);
        
        // Render appropriate content based on current state
        if (state->menu.displayMenu != MENU_NONE) {
            // Render menu interface
            drawMenu(state);
        } else {
            // Game rendering and logic
            
            // Calculate frame rate and update movement speeds
            calculateFPS(&state->app, &state->playerState);

            // Process continuous input (movement, etc.)
            keyboardInput(state);
            if (state->app.controller != NULL) {
                controllerInput(state);
            }

            // Render main game scene
            renderScene(state);

            // Render UI overlays
            itemFrame(state->app, state->playerState.selectFrame);
            
            // Render debug interfaces if enabled
            if (state->playerState.showState) {
                showStateInterface(&state->app, state->playerState, state->entityState);
            }
            if (state->playerState.showMap) {
                showMapInterface(state->app, state->mapState, state->playerState);
            }
        }

        // Present the rendered frame to screen
        SDL_RenderPresent(state->app.renderer);
    }

    // Cleanup and exit
    closeWindow(state);
    return 0;
} 