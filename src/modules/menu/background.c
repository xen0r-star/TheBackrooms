#include "menu.h"


void background(GameState *state, BackgroundType backgroundType) {
    // Validate input parameters
    if (!state) {
        fprintf(stderr, "Error: Invalid game state for background rendering\n");
        return;
    }

    if (backgroundType == BACKGROUND_MENU) {
        // Static variables for menu animation state persistence
        static bool mapGenerated = false;
        static PlayerState staticPlayerState = {
            {5.0, 5.0, 0.0},    // Player position in demo scene
            0.0, 0.0, 0.0, 0.0, // Movement and rotation values
            1,                   // Player speed
            false, false,        // Movement flags
            true, true          // State flags
        };
        static MapState staticMapState = {
            10, 10,             // Demo map dimensions
            NULL, NULL,         // Map arrays (allocated during generation)
            0                   // Launch type
        };

        // Generate demo map on first call
        if (!mapGenerated) {
            printf("Generating demo map for menu background...\n");
            generateMap(&staticMapState, staticMapState.mapWidth, staticMapState.mapHeight);
            spawnPlayerFromMap(&staticPlayerState, &staticMapState);
            mapGenerated = true;
        }

        // Animate camera rotation for dynamic background effect
        const float rotationSpeed = 0.025f;  // Radians per frame
        const float maxAngle = 360.0f;
        
        staticPlayerState.player.angle += rotationSpeed;
        if (staticPlayerState.player.angle > maxAngle) {
            staticPlayerState.player.angle = 0.0f;
        }
        
        // Update rendering direction vectors based on new angle
        calculateDirRender(staticPlayerState.player.angle, 
                          &state->graphics.renderCache.dirX, 
                          &state->graphics.renderCache.dirY);

        // Create temporary game state for background rendering
        GameState gameStateCopy = {
            .app = state->app,
            .playerState = staticPlayerState,
            .entityState = state->entityState,
            .mapState = staticMapState,
            .graphics = state->graphics,
            .settings = state->settings
        };

        // Render the animated 3D scene
        renderScene(&gameStateCopy);

    } else if (backgroundType == BACKGROUND_GAME) {
        // Render current game state as dimmed background for pause menu
        
        // Update screen texture with current game frame
        if (state->graphics.screenBuffersTexture && state->graphics.screenBuffers) {
            SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, 
                state->graphics.screenBuffers, 
                state->app.screenWidth * sizeof(Uint32)
            );
            
            // Render the game frame
            SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);
        }

        // Apply semi-transparent overlay to dim the background
        const Uint8 overlayAlpha = 190;  // Transparency level for dimming
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, overlayAlpha);
        
        SDL_Rect fullScreenRect = {
            0, 0, 
            state->app.screenWidth, 
            state->app.screenHeight
        };
        SDL_RenderFillRect(state->app.renderer, &fullScreenRect);
        
    } else {
        // Handle unexpected background type
        fprintf(stderr, "Warning: Unknown background type %d\n", backgroundType);
        
        // Fallback to solid color background
        SDL_SetRenderDrawColor(state->app.renderer, 20, 20, 20, 255);
        SDL_RenderClear(state->app.renderer);
    }
}