#include "graphics.h"


static int validateTexturePath(const char *path) {
    if (!path || strlen(path) == 0) {
        return -1;
    }
    
    // Check file extension (basic validation)
    const char *ext = strrchr(path, '.');
    if (!ext || (strcmp(ext, ".png") != 0 && strcmp(ext, ".jpg") != 0 && strcmp(ext, ".bmp") != 0)) {
        printf("Warning: Unsupported texture format: %s\n", path);
    }
    
    return 0;
}

static inline Uint32 convertPixelFormat(Uint32 pixel) {
    // Convert from RGBA to ARGB format
    return ((pixel & 0xFF)         << 24) |  // A
           (((pixel >> 8 ) & 0xFF) << 16) |  // R
           (((pixel >> 16) & 0xFF) << 8 ) |  // G
           ((pixel  >> 24) & 0xFF);          // B
}

static int loadSingleTexture(const char *texturePath, Uint32 *textureBuffer) {
    if (validateTexturePath(texturePath) != 0) {
        return -1;
    }

    // Load image surface
    SDL_Surface *surface = IMG_Load(texturePath);
    if (!surface) {
        printf("Error: Failed to load texture '%s': %s\n", texturePath, IMG_GetError());
        return -1;
    }

    // Validate texture dimensions
    if (surface->w != TEXTURE_SIZE || surface->h != TEXTURE_SIZE) {
        printf("Warning: Texture '%s' size is %dx%d, expected %dx%d. Scaling may occur.\n",
               texturePath, surface->w, surface->h, TEXTURE_SIZE, TEXTURE_SIZE);
    }

    // Convert and copy pixel data
    for (int y = 0; y < TEXTURE_SIZE; y++) {
        for (int x = 0; x < TEXTURE_SIZE; x++) {
            // Calculate source coordinates with bounds checking
            int srcX = (x * surface->w) / TEXTURE_SIZE;
            int srcY = (y * surface->h) / TEXTURE_SIZE;
            
            // Clamp to surface bounds
            if (srcX >= surface->w) srcX = surface->w - 1;
            if (srcY >= surface->h) srcY = surface->h - 1;
            
            // Get source pixel
            Uint32 pixel = ((Uint32*)surface->pixels)[srcY * surface->w + srcX];
            
            // Convert format and store
            textureBuffer[y * TEXTURE_SIZE + x] = convertPixelFormat(pixel);
        }
    }

    SDL_FreeSurface(surface);
    return 0;
}

static int allocateTextureBuffers(GameState *state) {
    // Allocate array of texture buffer pointers
    state->graphics.textureBuffers = (Uint32**)malloc(NUMBER_TEXTURES * sizeof(Uint32*));
    if (!state->graphics.textureBuffers) {
        printf("Error: Failed to allocate texture buffer array\n");
        return -1;
    }

    // Initialize pointers to NULL for safe cleanup
    for (int i = 0; i < NUMBER_TEXTURES; i++) {
        state->graphics.textureBuffers[i] = NULL;
    }

    // Allocate individual texture buffers
    size_t textureSize = TEXTURE_SIZE * TEXTURE_SIZE * sizeof(Uint32);
    for (int i = 0; i < NUMBER_TEXTURES; i++) {
        state->graphics.textureBuffers[i] = (Uint32*)malloc(textureSize);
        if (!state->graphics.textureBuffers[i]) {
            printf("Error: Failed to allocate texture buffer %d (%.2f KB)\n", 
                   i, textureSize / 1024.0f);
            
            // Cleanup previously allocated buffers
            for (int j = 0; j < i; j++) {
                free(state->graphics.textureBuffers[j]);
            }
            free(state->graphics.textureBuffers);
            return -1;
        }
    }

    return 0;
}


int initializationTextures(GameState *state) {
    if (!state) {
        return GRAPHICS_ERROR_INIT;
    }

    // Define texture file paths
    const char* texturePaths[NUMBER_TEXTURES] = {
        "textures/breadMat.png",    // Wall texture 1
        "textures/breadMat.png",    // Wall texture 2 (duplicate for now)
        "textures/floor.png",       // Floor texture
        "textures/ceiling.png",     // Ceiling texture
        "textures/e1.png"           // Entity texture
    };

    printf("Info: Loading %d textures...\n", NUMBER_TEXTURES);

    // Allocate texture buffer memory
    if (allocateTextureBuffers(state) != 0) {
        return GRAPHICS_ERROR_MEMORY;
    }

    // Load each texture file
    for (int i = 0; i < NUMBER_TEXTURES; i++) {
        printf("Info: Loading texture %d: %s\n", i, texturePaths[i]);
        
        if (loadSingleTexture(texturePaths[i], state->graphics.textureBuffers[i]) != 0) {
            printf("Error: Failed to load texture %d: %s\n", i, texturePaths[i]);
            
            // Cleanup on failure
            for (int j = 0; j <= i; j++) {
                if (state->graphics.textureBuffers[j]) {
                    free(state->graphics.textureBuffers[j]);
                }
            }
            free(state->graphics.textureBuffers);
            return GRAPHICS_ERROR_RESOURCE;
        }
    }

    printf("Info: All textures loaded successfully\n");
    return GRAPHICS_SUCCESS;
}