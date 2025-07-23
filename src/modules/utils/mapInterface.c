#include "utils.h"

void showMapInterface(const AppState appState, const MapState mapState, const PlayerState playerState) {
    SDL_Renderer *renderer = appState.renderer;
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    int windowWidth = mapState.mapWidth;
    int windowHeight = mapState.mapHeight;

    int drawSize = (screenHeight * 0.4) / windowHeight;

    float increment = ((1.5 - 1.0) / (windowHeight - 1));

    int playerX = (int)playerState.player.x;
    int playerY = (int)playerState.player.y;

    // Calcul du début de la fenêtre autour du joueur
    int startX = playerX - windowWidth / 2;
    int startY = playerY - windowHeight / 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (startX + windowWidth > mapState.mapWidth)  startX = mapState.mapWidth  - windowWidth;
    if (startY + windowHeight > mapState.mapHeight) startY = mapState.mapHeight - windowHeight;
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    // === Dessin du fond ===
    for (int y = 0; y < windowHeight; y++) {
        float deformation = 1.0 + y * increment;
        for (int x = 0; x < windowWidth; x++) {
            SDL_SetRenderDrawColor(renderer, 220, 180, 120, 255);
            SDL_Rect mapRect = {
                ((screenWidth - drawSize * deformation * windowWidth) / 2) + x * drawSize * deformation,
                (screenHeight - drawSize * windowHeight - 25) + y * drawSize,
                drawSize * deformation + 1,
                drawSize
            };
            SDL_RenderFillRect(renderer, &mapRect);
        }
    }

    // === Dessin de la carte visible ===
    for (int y = 0; y < windowHeight; y++) {
        float deformation = 1.0 + y * increment;
        for (int x = 0; x < windowWidth; x++) {
            int mapX = startX + x;
            int mapY = startY + y;

            if (mapState.mapDiscovered[mapY][mapX] == 1) {
                switch (mapState.map[mapY][mapX]) {
                    case 1:  SDL_SetRenderDrawColor(renderer,  54,  43,  26, 255); break;
                    case 2:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 3:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 4:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    case 5:  SDL_SetRenderDrawColor(renderer,  92,  70,  37, 255); break;
                    default: SDL_SetRenderDrawColor(renderer, 160, 125,  72, 255); break;
                }

                SDL_Rect block = {
                    ((screenWidth - drawSize * deformation * windowWidth) / 2) + x * drawSize * deformation,
                    (screenHeight - drawSize * windowHeight - 25) + y * drawSize,
                    drawSize * deformation + 1,
                    drawSize
                };
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    // === Dessin du joueur dans la fenêtre ===
    float deformation = 1.0 + (playerY - startY) * increment;

    SDL_SetRenderDrawColor(renderer, 196, 65, 65, 255);
    SDL_Rect playerRect = {
        ((screenWidth - drawSize * deformation * windowWidth) / 2) + (playerX - startX) * drawSize * deformation,
        (screenHeight - drawSize * windowHeight - 25) + (playerY - startY) * drawSize,
        4, 4
    };
    SDL_RenderFillRect(renderer, &playerRect);
}