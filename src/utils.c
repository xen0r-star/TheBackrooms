#include "utils.h"
#include "render.h"


void calculateFPS() {
    fps = (double) (clock() - startTime) / CLOCKS_PER_SEC;
    startTime = clock();

    playerMoveSpeed = moveSpeed * fps;
    playerRotateSpeed = rotateSpeed * fps;
    fps = (int)(1. / fps);
}


int renderText(int x, int y, int w, int h, const char *text, SDL_Color color) {
    SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(font, text, color);
    if (!surfaceMessage) {
        printf("Erreur lors de la création de la surface texte: %s\n", TTF_GetError());
        return 1;
    }

    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    if (!message) {
        printf("Erreur lors de la création de la texture texte: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect message_rect;
    message_rect.x = x;  // Position en x
    message_rect.y = y;  // Position en y
    message_rect.w = w;  // Largeur du texte
    message_rect.h = h;   // Hauteur du texte

    SDL_RenderCopy(renderer, message, NULL, &message_rect);

    return 0;
}



void showStateInterface() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
    SDL_Rect block = {10, 10, SCREEN_WIDTH / 3 - 10, SCREEN_HEIGHT / 2 - 10};
    SDL_RenderFillRect(renderer, &block);

    static char fpsText[25];
    char text[25];
    if ((startTime - previousTime) * 1000 / CLOCKS_PER_SEC >= 500) {
        previousTime = clock();
        sprintf(fpsText, "FPS: %.0f", fps);
    }
    
    renderText(30, 30, 100, 25, fpsText, (SDL_Color){255, 255, 255, 255});

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect wallRect = {30, 70, SCREEN_WIDTH / 3 - 60, 2};
    SDL_RenderFillRect(renderer, &wallRect);

    sprintf(text, "Colision: %1d", colision);
    renderText(30, 80, 100, 25, text, (SDL_Color){255, 255, 255, 255});

    sprintf(text, "Texture: %1d", showTextures);
    renderText(30, 110, 92, 25, text, (SDL_Color){255, 255, 255, 255});

    sprintf(text, "Map: %1d", showMap);
    renderText(30, 140, 62, 25, text, (SDL_Color){255, 255, 255, 255});
}



void showMapInterface() {
    int drawSize = (SCREEN_HEIGHT * 0.4) / mapHeight;
    float increment = ((1.5 - 1.0) / (mapHeight - 1));



    for (int y = 0; y < 60; y++) {
        float deformation = 1.0 + y * increment;

        for (int x = 0; x < 70; x++) {
            SDL_SetRenderDrawColor(renderer, 220, 180, 120, 255);

            SDL_Rect mapRect = {
                ((SCREEN_WIDTH - drawSize * deformation * 70) / 2) + x * drawSize * deformation,
                (SCREEN_HEIGHT - drawSize * 60 - 25) + y * drawSize,
                drawSize * deformation + 1,
                drawSize
            };
            SDL_RenderFillRect(renderer, &mapRect);
        }
    }


    for (int y = 0; y < mapHeight; y++) {
        float deformation = 1.0 + y * increment;

        for (int x = 0; x < mapWidth; x++) {
            if (mapDiscovered[y][x] == 1) {
                switch (map[y][x]) {
                    case 1:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 2:  SDL_SetRenderDrawColor(renderer,  54,  43,  26, 255); break;
                    case 3:  SDL_SetRenderDrawColor(renderer,  54,  43,  26, 255); break;
                    case 4:  SDL_SetRenderDrawColor(renderer,  54,  43,  26, 255); break;
                    case 5:  SDL_SetRenderDrawColor(renderer,  54,  43,  26, 255); break;
                    default: SDL_SetRenderDrawColor(renderer, 160, 125,  72, 255); break;
                }

                SDL_Rect block = {
                    ((SCREEN_WIDTH - drawSize * deformation * mapWidth) / 2) + x * drawSize * deformation,  // X
                    (SCREEN_HEIGHT - drawSize * mapHeight - 35) + y * drawSize,                             // Y
                    drawSize * deformation + 1,                                                                     // W
                    drawSize                                                                                        // H
                };
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    float deformation = 1.0 + player.y * increment;

    SDL_SetRenderDrawColor(renderer, 196, 65, 65, 255);
    SDL_Rect playerRect = { 
        ((SCREEN_WIDTH - drawSize * deformation * mapWidth) / 2) + player.x * drawSize * deformation, 
        (SCREEN_HEIGHT - drawSize * mapHeight - 35) + player.y * drawSize, 
        4, 4 
    };
    SDL_RenderFillRect(renderer, &playerRect);
}



void itemFrame(int selectFrame) {
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
            ((SCREEN_WIDTH - 5 * 50) / 2) + i * 50, 
            SCREEN_HEIGHT - 50 - 10, 
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