#include "graphics.h"


static int validateScreenParams(const GameState *state) {
    if (!state || !state->app.renderer) {
        return -1;
    }
    
    if (state->app.screenWidth <= 0 || state->app.screenHeight <= 0) {
        return -1;
    }
    
    const int MAX_RESOLUTION = 7680; // 8K limit
    if (state->app.screenWidth > MAX_RESOLUTION || state->app.screenHeight > MAX_RESOLUTION) {
        return -1;
    }
    
    return 0;
}

static int createScreenTexture(GameState *state) {
    // Create streaming texture for pixel-level rendering
    state->graphics.screenBuffersTexture = SDL_CreateTexture(
        state->app.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        state->app.screenWidth, 
        state->app.screenHeight
    );

    if (!state->graphics.screenBuffersTexture) {
        printf("Error: Failed to create screen texture: %s\n", SDL_GetError());
        return -1;
    }
    
    return 0;
}

static int allocateScreenBuffer(GameState *state) {
    size_t bufferSize = state->app.screenWidth * state->app.screenHeight * sizeof(Uint32);
    
    state->graphics.screenBuffers = (Uint32 *)malloc(bufferSize);
    if (!state->graphics.screenBuffers) {
        printf("Error: Failed to allocate screen buffer (%.2f MB)\n", bufferSize / (1024.0f * 1024.0f));
        return -1;
    }
    
    // Initialize buffer to black
    memset(state->graphics.screenBuffers, 0, bufferSize);
    
    return 0;
}

static int allocateZBuffer(GameState *state) {
    size_t bufferSize = sizeof(float) * state->app.screenWidth;
    
    state->entityState.zBuffer = (float *)malloc(bufferSize);
    if (!state->entityState.zBuffer) {
        printf("Error: Failed to allocate z-buffer (%.2f KB)\n", bufferSize / 1024.0f);
        return -1;
    }
    
    // Initialize z-buffer to maximum distance
    for (int i = 0; i < state->app.screenWidth; i++) {
        state->entityState.zBuffer[i] = 1000.0f; // Far clipping plane
    }
    
    return 0;
}

static int initializeRenderCache(GameState *state) {
    // Calculate field of view for rendering pipeline
    state->graphics.renderCache.fovRender = calculateFovRender(state->settings.fov);
    
    // Initialize direction vectors (will be updated per frame)
    state->graphics.renderCache.dirX = 1.0f; // Facing right initially
    state->graphics.renderCache.dirY = 0.0f;
    
    return 0;
}


int initializationScreen(GameState *state) {
    // Validate input parameters
    if (validateScreenParams(state) != 0) {
        return GRAPHICS_ERROR_INIT;
    }

    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;
    
    printf("Info: Initializing screen buffers for %dx%d resolution\n", screenWidth, screenHeight);

    // Create SDL rendering texture
    if (createScreenTexture(state) != 0) {
        return GRAPHICS_ERROR_MEMORY;
    }

    // Allocate screen pixel buffer
    if (allocateScreenBuffer(state) != 0) {
        SDL_DestroyTexture(state->graphics.screenBuffersTexture);
        return GRAPHICS_ERROR_MEMORY;
    }

    // Allocate depth buffer for 3D rendering
    if (allocateZBuffer(state) != 0) {
        free(state->graphics.screenBuffers);
        SDL_DestroyTexture(state->graphics.screenBuffersTexture);
        return GRAPHICS_ERROR_MEMORY;
    }

    // Initialize rendering cache
    if (initializeRenderCache(state) != 0) {
        free(state->entityState.zBuffer);
        free(state->graphics.screenBuffers);
        SDL_DestroyTexture(state->graphics.screenBuffersTexture);
        return GRAPHICS_ERROR_INIT;
    }

    printf("Info: Screen initialization completed successfully\n");
    return GRAPHICS_SUCCESS;
}