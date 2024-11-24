#include "utils.h"
#include "render.h"


void calculateFPS(GameState *state) {
    state->app.fps = (double) (clock() - state->app.startTime) / CLOCKS_PER_SEC;
    state->app.startTime = clock();

    state->playerState.playerMoveSpeed = state->playerState.moveSpeed * state->app.fps;
    state->playerState.playerRotateSpeed = state->playerState.rotateSpeed * state->app.fps;
    state->app.fps = (int)(1. / state->app.fps);
}


int renderText(GameState *state, int x, int y, int w, int h, const char *text, SDL_Color color) {
    SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(state->app.font, text, color);
    if (!surfaceMessage) {
        printf("Erreur lors de la création de la surface texte: %s\n", TTF_GetError());
        return 1;
    }

    state->app.message = SDL_CreateTextureFromSurface(state->app.renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    if (!state->app.message) {
        printf("Erreur lors de la création de la texture texte: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect message_rect;
    message_rect.x = x;  // Position en x
    message_rect.y = y;  // Position en y
    message_rect.w = w;  // Largeur du texte
    message_rect.h = h;   // Hauteur du texte

    SDL_RenderCopy(state->app.renderer, state->app.message, NULL, &message_rect);

    return 0;
}



void showStateInterface(GameState *state) {
    SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 190);
    SDL_Rect block = {10, 10, state->app.screenWidth / 3 - 10, state->app.screenHeight / 2 - 10};
    SDL_RenderFillRect(state->app.renderer, &block);

    static char fpsText[25];
    char text[25];
    if ((state->app.startTime - state->app.previousTime) * 1000 / CLOCKS_PER_SEC >= 500) {
        state->app.previousTime = clock();
        sprintf(fpsText, "FPS: %.0f", state->app.fps);
    }
    
    renderText(state, 30, 30, 100, 25, fpsText, (SDL_Color){255, 255, 255, 255});

    SDL_SetRenderDrawColor(state->app.renderer, 255, 255, 255, 255);
    SDL_Rect wallRect = {30, 70, state->app.screenWidth / 3 - 60, 2};
    SDL_RenderFillRect(state->app.renderer, &wallRect);

    sprintf(text, "Colision: %1d", state->playerState.collision);
    renderText(state, 30, 80, 100, 25, text, (SDL_Color){255, 255, 255, 255});

    sprintf(text, "Texture: %1d", state->playerState.showTextures);
    renderText(state, 30, 110, 92, 25, text, (SDL_Color){255, 255, 255, 255});

    sprintf(text, "Map: %1d", state->playerState.showMap);
    renderText(state, 30, 140, 62, 25, text, (SDL_Color){255, 255, 255, 255});
}



void showMapInterface(GameState *state) {
    int drawSize = (state->app.screenHeight * 0.4) / state->mapState.mapHeight;
    float increment = ((1.5 - 1.0) / (state->mapState.mapHeight - 1));



    for (int y = 0; y < 60; y++) {
        float deformation = 1.0 + y * increment;

        for (int x = 0; x < 70; x++) {
            SDL_SetRenderDrawColor(state->app.renderer, 220, 180, 120, 255);

            SDL_Rect mapRect = {
                ((state->app.screenWidth - drawSize * deformation * 70) / 2) + x * drawSize * deformation,
                (state->app.screenHeight - drawSize * 60 - 25) + y * drawSize,
                drawSize * deformation + 1,
                drawSize
            };
            SDL_RenderFillRect(state->app.renderer, &mapRect);
        }
    }


    for (int y = 0; y < state->mapState.mapHeight; y++) {
        float deformation = 1.0 + y * increment;

        for (int x = 0; x < state->mapState.mapWidth; x++) {
            if (state->mapState.mapDiscovered[y][x] == 1) {
                switch (state->mapState.map[y][x]) {
                    case 1:  SDL_SetRenderDrawColor(state->app.renderer,  92,  70,  37, 255); break;
                    case 2:  SDL_SetRenderDrawColor(state->app.renderer,  54,  43,  26, 255); break;
                    case 3:  SDL_SetRenderDrawColor(state->app.renderer,  54,  43,  26, 255); break;
                    case 4:  SDL_SetRenderDrawColor(state->app.renderer,  54,  43,  26, 255); break;
                    case 5:  SDL_SetRenderDrawColor(state->app.renderer,  54,  43,  26, 255); break;
                    default: SDL_SetRenderDrawColor(state->app.renderer, 160, 125,  72, 255); break;
                }

                SDL_Rect block = {
                    ((state->app.screenWidth - drawSize * deformation * state->mapState.mapWidth) / 2) + x * drawSize * deformation,  // X
                    (state->app.screenHeight - drawSize * state->mapState.mapHeight - 35) + y * drawSize,                             // Y
                    drawSize * deformation + 1,                                                                     // W
                    drawSize                                                                                        // H
                };
                SDL_RenderFillRect(state->app.renderer, &block);
            }
        }
    }

    float deformation = 1.0 + state->playerState.player.y * increment;

    SDL_SetRenderDrawColor(state->app.renderer, 196, 65, 65, 255);
    SDL_Rect playerRect = { 
        ((state->app.screenWidth - drawSize * deformation * state->mapState.mapWidth) / 2) + state->playerState.player.x * drawSize * deformation, 
        (state->app.screenHeight - drawSize * state->mapState.mapHeight - 35) + state->playerState.player.y * drawSize, 
        4, 4 
    };
    SDL_RenderFillRect(state->app.renderer, &playerRect);
}



void itemFrame(GameState *state, int selectFrame) {
    SDL_Surface *frameSurface = IMG_Load("textures/frame.png");
    if (!frameSurface) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *frameTexture = SDL_CreateTextureFromSurface(state->app.renderer, frameSurface);
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

    SDL_Texture *frameSelectTexture = SDL_CreateTextureFromSurface(state->app.renderer, frameSelectSurface);
    SDL_FreeSurface(frameSelectSurface);
    if (!frameSelectTexture) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        return;
    }


    for (int i = 0; i < 5; i++) {
        SDL_Rect destRect = { 
            ((state->app.screenWidth - 5 * 50) / 2) + i * 50, 
            state->app.screenHeight - 50 - 10, 
            50, 
            50 
        };

        if (i + 1 == selectFrame) {
            SDL_RenderCopy(state->app.renderer, frameSelectTexture, NULL, &destRect);
        } else {
            SDL_RenderCopy(state->app.renderer, frameTexture, NULL, &destRect);
        }
    }

    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(frameSelectTexture);
}