#include "utils.h"

void itemFrame(const AppState appState, int selectFrame) {
    SDL_Renderer *renderer = appState.renderer;
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    SDL_Surface *inventoryBarSurface = IMG_Load("textures/inventoryBar.png");
    if (!inventoryBarSurface) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *inventoryBar = SDL_CreateTextureFromSurface(renderer, inventoryBarSurface);
    SDL_FreeSurface(inventoryBarSurface);
    if (!inventoryBar) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        return;
    }


    SDL_Surface *frameSelectSurface = IMG_Load("textures/frameSelect.png");
    if (!frameSelectSurface) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *frameSelect = SDL_CreateTextureFromSurface(renderer, frameSelectSurface);
    SDL_FreeSurface(frameSelectSurface);
    if (!frameSelect) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        return;
    }

    
    SDL_RenderCopy(renderer, inventoryBar, NULL, &(SDL_Rect) {
        (screenWidth - 358) / 2, 
        screenHeight - 64 - 20, 
        358, 
        64 
    });

    SDL_RenderCopy(renderer, frameSelect, NULL, &(SDL_Rect) {
        ((screenWidth - 358) / 2) + 49 * (selectFrame - 1), 
        screenHeight - 64 - 20, 
        64, 
        64 
    });
    

    SDL_DestroyTexture(inventoryBar);
    SDL_DestroyTexture(frameSelect);
}