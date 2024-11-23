#include "window.h"

int initializationWindow() {
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
                                          screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
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
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    SDL_Surface *iconSurface = IMG_Load(ICON_FILE);
    if (!iconSurface) {
        printf("Erreur lors du chargement de l'icône : %s\n", IMG_GetError());
        return 1;
    }

    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);

    font = TTF_OpenFont(FONT_FILE, 72);
    if (!font) {
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
        return 1;
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);
    keystate = SDL_GetKeyboardState(NULL);
    
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (SDL_IsGameController(i)) {
                controller = SDL_GameControllerOpen(i);
            }
        }

        if (controller == NULL) {
            printf("Could not open gamecontroller! SDL_Error: %s\n", SDL_GetError());
        }
    }

    return 0;
}

int closeWindow() {
    // Libérer la mémoire pour chaque texture
    for (int i = 0; i < NUMBER_TEXTURES; i++) {
        free(textureBuffers[i]);
    }

    free(textureBuffers);

    if (controller != NULL) {
        SDL_GameControllerClose(controller);
        controller = NULL;
    }


    // Nettoyage de SDL
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(message);
    TTF_CloseFont(font);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}