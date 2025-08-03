#include "graphics.h"


static int validateScreenDimensions(int width, int height) {
    const int MIN_RESOLUTION = 640;
    const int MAX_RESOLUTION = 7680; // 8K resolution
    
    if (width < MIN_RESOLUTION || width > MAX_RESOLUTION ||
        height < MIN_RESOLUTION || height > MAX_RESOLUTION) {
        return -1;
    }
    
    return 0;
}

static int initializeSDLSubsystems(void) {
    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error: SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }
    
    // Initialize SDL_ttf for font rendering
    if (TTF_Init() == -1) {
        printf("Error: SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }
    
    return 0;
}

static int setupScreenDimensions(GameState *state) {
    // Get usable display bounds (excluding taskbar, dock, etc.)
    SDL_Rect usableBounds;
    if (SDL_GetDisplayUsableBounds(0, &usableBounds) == 0) {
        state->app.screenWidth = usableBounds.w;
        state->app.screenHeight = usableBounds.h;
        
        // Validate dimensions
        if (validateScreenDimensions(state->app.screenWidth, state->app.screenHeight) != 0) {
            // Fallback to safe resolution
            state->app.screenWidth = 1920;
            state->app.screenHeight = 1080;
            printf("Warning: Using fallback resolution 1920x1080\n");
        }
    } else {
        // Fallback resolution if display bounds detection fails
        state->app.screenWidth = 1920;
        state->app.screenHeight = 1080;
        printf("Warning: Could not detect display bounds, using fallback resolution\n");
    }
    
    return 0;
}

static int createSDLWindow(GameState *state) {
    // Create SDL window
    state->app.window = SDL_CreateWindow(WINDOW_TITLE,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          state->app.screenWidth, 
                                          state->app.screenHeight, 
                                          SDL_WINDOW_BORDERLESS);
    if (state->app.window == NULL) {
        printf("Error: SDL_CreateWindow failed: %s\n", SDL_GetError());
        return -1;
    }

    // Position window at top-left corner
    SDL_SetWindowPosition(state->app.window, 0, 0);
    
    return 0;
}

static int createSDLRenderer(GameState *state) {
    // Create hardware-accelerated renderer
    state->app.renderer = SDL_CreateRenderer(state->app.window, -1, SDL_RENDERER_ACCELERATED);
    if (state->app.renderer == NULL) {
        printf("Error: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return -1;
    }
    
    // Enable alpha blending for transparency effects
    SDL_SetRenderDrawBlendMode(state->app.renderer, SDL_BLENDMODE_BLEND);
    
    return 0;
}

static int setupWindowIcon(GameState *state) {
    SDL_Surface *iconSurface = IMG_Load(ICON_FILE);
    if (!iconSurface) {
        printf("Warning: Failed to load window icon: %s\n", IMG_GetError());
        return -1; // Non-critical error
    }

    SDL_SetWindowIcon(state->app.window, iconSurface);
    SDL_FreeSurface(iconSurface);
    
    return 0;
}

static int initializeFonts(GameState *state) {
    Font fontType[TEXT_TYPE_COUNT] = {
        {FONT_ROBOTO, 26}, 
        {FONT_ROBOTO, 28}, 
        {FONT_ROBOTO, 32}, 
        {FONT_ROBOTO, 36}, 
        {FONT_ROBOTO, 48}, 
        {FONT_ROBOTO, 72}, 
        {FONT_JERSEY, 94}
    };

    for (int i = 0; i < TEXT_TYPE_COUNT; i++) {
        state->app.text[i] = TTF_OpenFont(fontType[i].family, fontType[i].size);
        if (!state->app.text[i]) {
            printf("Error: Failed to load font %s size %d: %s\n", 
                   fontType[i].family, fontType[i].size, TTF_GetError());
            
            // Cleanup previously loaded fonts
            for (int j = 0; j < i; j++) {
                TTF_CloseFont(state->app.text[j]);
            }
            return -1;
        }
    }
    
    return 0;
}

static int initializeInputDevices(GameState *state) {
    // Disable relative mouse mode (for menu navigation)
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    // Get keyboard state pointer
    state->app.keystate = SDL_GetKeyboardState(NULL);
    
    // Initialize game controller support
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        printf("Warning: Could not initialize game controller support: %s\n", SDL_GetError());
    } else {
        // Find and open first available game controller
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (SDL_IsGameController(i)) {
                state->app.controller = SDL_GameControllerOpen(i);
                if (state->app.controller) {
                    printf("Info: Game controller detected and initialized\n");
                    break;
                }
            }
        }
    }
    
    return 0;
}


int initializationWindow(GameState *state) {
    // Validate input parameter
    if (!state) {
        return GRAPHICS_ERROR_INIT;
    }

    // Initialize SDL subsystems
    if (initializeSDLSubsystems() != 0) {
        return GRAPHICS_ERROR_INIT;
    }

    // Setup screen dimensions
    if (setupScreenDimensions(state) != 0) {
        TTF_Quit();
        SDL_Quit();
        return GRAPHICS_ERROR_INIT;
    }

    // Create SDL window
    if (createSDLWindow(state) != 0) {
        TTF_Quit();
        SDL_Quit();
        return GRAPHICS_ERROR_INIT;
    }

    // Create SDL renderer
    if (createSDLRenderer(state) != 0) {
        SDL_DestroyWindow(state->app.window);
        TTF_Quit();
        SDL_Quit();
        return GRAPHICS_ERROR_INIT;
    }

    // Setup window icon (non-critical)
    setupWindowIcon(state);

    // Initialize fonts
    if (initializeFonts(state) != 0) {
        SDL_DestroyRenderer(state->app.renderer);
        SDL_DestroyWindow(state->app.window);
        TTF_Quit();
        SDL_Quit();
        return GRAPHICS_ERROR_INIT;
    }

    // Initialize input devices (non-critical)
    initializeInputDevices(state);

    return GRAPHICS_SUCCESS;
}


int closeWindow(GameState *state) {
    if (!state) {
        return GRAPHICS_ERROR_INIT;
    }

    // Free texture memory safely
    if (state->graphics.textureBuffers) {
        for (int i = 0; i < NUMBER_TEXTURES; i++) {
            if (state->graphics.textureBuffers[i]) {
                free(state->graphics.textureBuffers[i]);
                state->graphics.textureBuffers[i] = NULL;
            }
        }
        free(state->graphics.textureBuffers);
        state->graphics.textureBuffers = NULL;
    }

    // Free sprite memory
    if (state->entityState.sprites) {
        free(state->entityState.sprites);
        state->entityState.sprites = NULL;
    }

    // Free z-buffer memory
    if (state->entityState.zBuffer) {
        free(state->entityState.zBuffer);
        state->entityState.zBuffer = NULL;
    }

    // Close game controller if open
    if (state->app.controller != NULL) {
        SDL_GameControllerClose(state->app.controller);
        state->app.controller = NULL;
    }

    // Close all fonts
    for (int i = 0; i < TEXT_TYPE_COUNT; i++) {
        if (state->app.text[i]) {
            TTF_CloseFont(state->app.text[i]);
            state->app.text[i] = NULL;
        }
    }
    
    // Destroy SDL objects in proper order
    if (state->app.renderer) {
        SDL_DestroyRenderer(state->app.renderer);
        state->app.renderer = NULL;
    }
    
    if (state->app.window) {
        SDL_DestroyWindow(state->app.window);
        state->app.window = NULL;
    }

    // Quit SDL subsystems
    TTF_Quit();
    SDL_Quit();

    // Free main state structure
    free(state);

    return GRAPHICS_SUCCESS;
}

bool hasWindowResize(AppState *appState) {
    if (!appState || !appState->window) {
        return false;
    }

    int newWidth, newHeight;
    SDL_GetWindowSize(appState->window, &newWidth, &newHeight);

    return (newWidth != appState->screenWidth || newHeight != appState->screenHeight);
}