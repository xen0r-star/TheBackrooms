#include "texture.h"

void initializationTextures() {
    const char* texturePaths[numberTextures] = {
        "textures/breadMat.png",
        "textures/breadMat.png",

        "textures/floor.png",
        "textures/ceiling.png",
    };

    textureBuffers = (Uint32**)malloc(numberTextures * sizeof(Uint32*));
    for (int i = 0; i < numberTextures; i++) {
        SDL_Surface* surface = IMG_Load(texturePaths[i]);
        textureBuffers[i] = (Uint32*)malloc(textureSize * textureSize * sizeof(Uint32));
    
        for (int y = 0; y < textureSize; y++) {
            for (int x = 0; x < textureSize; x++) {
                Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
                pixel = ((pixel & 0xFF)         << 24) // RGBA => ABGR
                    |   (((pixel >> 8 ) & 0xFF) << 16) 
                    |   (((pixel >> 16) & 0xFF) << 8 ) 
                    |   ((pixel  >> 24) & 0xFF);
                textureBuffers[i][y * textureSize + x] = pixel;
            }
        }

        SDL_FreeSurface(surface);
    }


    // Création de la texture de rendu
    screenBuffersTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT
    );

    screenBuffers = (Uint32 *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}