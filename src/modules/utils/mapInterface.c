#include "utils.h"

void showMapInterface(const AppState appState, const MapState mapState, const PlayerState playerState) {
    SDL_Renderer *renderer = appState.renderer;
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    int drawSize = (screenHeight * 0.4) / mapState.mapHeight;
    float increment = ((1.5 - 1.0) / (mapState.mapHeight - 1));

    for (int y = 0; y < 60; y++) {
        float deformation = 1.0 + y * increment;

        for (int x = 0; x < 70; x++) {
            SDL_SetRenderDrawColor(renderer, 220, 180, 120, 255);

            SDL_Rect mapRect = {
                ((screenWidth - drawSize * deformation * 70) / 2) + x * drawSize * deformation,
                (screenHeight - drawSize * 60 - 25) + y * drawSize,
                drawSize * deformation + 1,
                drawSize
            };
            SDL_RenderFillRect(renderer, &mapRect);
        }
    }


    for (int y = 0; y < mapState.mapHeight; y++) {
        float deformation = 1.0 + y * increment;

        for (int x = 0; x < mapState.mapWidth; x++) {
            if (mapState.mapDiscovered[y][x] == 1) {
                switch (mapState.map[y][x]) {
                    case 1:  SDL_SetRenderDrawColor(renderer,  54,  43,  26, 255); break;
                    case 2:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 3:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 4:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 5:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    default: SDL_SetRenderDrawColor(renderer, 160, 125,  72, 255); break;
                }

                SDL_Rect block = {
                    ((screenWidth - drawSize * deformation * mapState.mapWidth) / 2) + x * drawSize * deformation,  // X
                    (screenHeight - drawSize * mapState.mapHeight - 35) + y * drawSize,                             // Y
                    drawSize * deformation + 1,                                                                     // W
                    drawSize                                                                                        // H
                };
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    float deformation = 1.0 + playerState.player.y * increment;

    SDL_SetRenderDrawColor(renderer, 196, 65, 65, 255);
    SDL_Rect playerRect = { 
        ((screenWidth - drawSize * deformation * mapState.mapWidth) / 2) + playerState.player.x * drawSize * deformation, 
        (screenHeight - drawSize * mapState.mapHeight - 35) + playerState.player.y * drawSize, 
        4, 4 
    };
    SDL_RenderFillRect(renderer, &playerRect);
}