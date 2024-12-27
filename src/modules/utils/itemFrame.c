#include "utils.h"

void itemFrame(const AppState appState, int selectFrame) {
    SDL_Renderer *renderer = appState.renderer;
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    SDL_Surface *frameSurface = IMG_Load("textures/frame.png");
    if (!frameSurface) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *frameTexture = SDL_CreateTextureFromSurface(renderer, frameSurface);
    SDL_FreeSurface(frameSurface);
    if (!frameTexture) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        return;
    }


    SDL_Surface *frameSelectSurface = IMG_Load("textures/frameSelect.png");
    if (!frameSelectSurface) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *frameSelectTexture = SDL_CreateTextureFromSurface(renderer, frameSelectSurface);
    SDL_FreeSurface(frameSelectSurface);
    if (!frameSelectTexture) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        return;
    }


    for (int i = 0; i < 5; i++) {
        SDL_Rect destRect = { 
            ((screenWidth - 5 * 50) / 2) + i * 50, 
            screenHeight - 50 - 10, 
            50, 
            50 
        };

        if (i + 1 == selectFrame) {
            SDL_RenderCopy(renderer, frameSelectTexture, NULL, &destRect);
        } else {
            SDL_RenderCopy(renderer, frameTexture, NULL, &destRect);
        }
    }

    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(frameSelectTexture);
}