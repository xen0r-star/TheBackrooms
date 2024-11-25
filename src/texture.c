#include "texture.h"

int initializationTextures(GameState *state) {
    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;

    const char* texturePaths[NUMBER_TEXTURES] = {
        "textures/breadMat.png",
        "textures/breadMat.png",

        "textures/floor.png",
        "textures/ceiling.png",
    };

    state->graphics.textureBuffers = (Uint32**)malloc(NUMBER_TEXTURES * sizeof(Uint32*));
    for (int i = 0; i < NUMBER_TEXTURES; i++) {
        SDL_Surface* surface = IMG_Load(texturePaths[i]);
        state->graphics.textureBuffers[i] = (Uint32*)malloc(TEXTURE_SIZE * TEXTURE_SIZE * sizeof(Uint32));
    
        for (int y = 0; y < TEXTURE_SIZE; y++) {
            for (int x = 0; x < TEXTURE_SIZE; x++) {
                Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
                pixel = ((pixel & 0xFF)         << 24) // RGBA => ABGR
                    |   (((pixel >> 8 ) & 0xFF) << 16) 
                    |   (((pixel >> 16) & 0xFF) << 8 ) 
                    |   ((pixel  >> 24) & 0xFF);
                state->graphics.textureBuffers[i][y * TEXTURE_SIZE + x] = pixel;
            }
        }

        SDL_FreeSurface(surface);
    }


    // Création de la texture de rendu
    state->graphics.screenBuffersTexture = SDL_CreateTexture(
        state->app.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight
    );

    state->graphics.screenBuffers = (Uint32 *)malloc(screenWidth * screenHeight * sizeof(Uint32));

    return 0;
}