#include "window.h"

int initializationWindow(GameState *state) {
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
    state->app.window = SDL_CreateWindow(WINDOW_TITLE,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          state->app.screenWidth, state->app.screenHeight, SDL_WINDOW_RESIZABLE);
    if (state->app.window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer SDL
    state->app.renderer = SDL_CreateRenderer(state->app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (state->app.renderer == NULL) {
        SDL_DestroyWindow(state->app.window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_SetRenderDrawBlendMode(state->app.renderer, SDL_BLENDMODE_BLEND);


    SDL_Surface *iconSurface = IMG_Load(ICON_FILE);
    if (!iconSurface) {
        printf("Erreur lors du chargement de l'icône : %s\n", IMG_GetError());
        return 1;
    }

    SDL_SetWindowIcon(state->app.window, iconSurface);
    SDL_FreeSurface(iconSurface);

    state->app.font = TTF_OpenFont(FONT_FILE, 72);
    if (!state->app.font) {
        printf("Erreur lors du chargement de la police: %s\n", TTF_GetError());
        return 1;
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);
    state->app.keystate = SDL_GetKeyboardState(NULL);
    
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (SDL_IsGameController(i)) {
                state->app.controller = SDL_GameControllerOpen(i);
            }
        }

        if (state->app.controller == NULL) {
            printf("Could not open gamecontroller! SDL_Error: %s\n", SDL_GetError());
        }
    }

    return 0;
}



int closeWindow(GameState *state) {
    // Libérer la mémoire pour chaque texture
    for (int i = 0; i < NUMBER_TEXTURES; i++) {
        free(state->graphics.textureBuffers[i]);
    }

    free(state->graphics.textureBuffers);

    if (state->app.controller != NULL) {
        SDL_GameControllerClose(state->app.controller);
        state->app.controller = NULL;
    }


    // Nettoyage de SDL
    SDL_DestroyTexture(state->menu.backgroundTexture);
    SDL_DestroyTexture(state->app.message);
    TTF_CloseFont(state->app.font);
    
    SDL_DestroyRenderer(state->app.renderer);
    SDL_DestroyWindow(state->app.window);

    TTF_Quit();
    SDL_Quit();

    free(state);

    return 0;
}