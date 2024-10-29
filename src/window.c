#include "window.h"

int createWindow() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur lors de l'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }

    // Création de la fenêtre SDL
    window = SDL_CreateWindow(WINDOW_TITLE,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer SDL
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    SDL_Surface *iconSurface = IMG_Load(iconFile);
    if (!iconSurface) {
        printf("Erreur lors du chargement de l'icône : %s\n", IMG_GetError());
        return 1;
    }

    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);

    font = TTF_OpenFont(fontFile, 72);
    if (!font) {
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
        return 1;
    }

    return 0;
}

int closeWindow() {
    // Libérer la mémoire pour chaque texture
    for (int i = 0; i < numberTextures; i++) {
        free(textureBuffers[i]);
    }

    free(textureBuffers);


    // Nettoyage de SDL
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(message);
    TTF_CloseFont(font);
    SDL_FreeCursor(pointerCursor);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}