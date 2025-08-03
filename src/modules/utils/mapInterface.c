#include "utils.h"


void showMapInterface(const AppState appState, const MapState mapState, const PlayerState playerState) {
    // Validate input parameters
    if (!appState.renderer) {
        fprintf(stderr, "Error: Invalid renderer for map interface\n");
        return;
    }
    
    if (!mapState.map || !mapState.mapDiscovered) {
        fprintf(stderr, "Error: Invalid map data for map interface\n");
        return;
    }

    SDL_Renderer *renderer = appState.renderer;
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    // Map window dimensions
    int windowWidth = mapState.mapWidth;
    int windowHeight = mapState.mapHeight;
    
    // Ensure minimum window size
    if (windowWidth <= 0 || windowHeight <= 0) {
        fprintf(stderr, "Error: Invalid map dimensions for interface\n");
        return;
    }

    // Calculate cell draw size (40% of screen height divided by window height)
    int drawSize = (int)((screenHeight * MINIMAP_SCREEN_RATIO) / windowHeight);
    if (drawSize < 1) drawSize = 1; // Ensure minimum draw size

    // Perspective deformation increment for visual depth
    float increment = (MINIMAP_MAX_DEFORMATION - 1.0f) / (windowHeight - 1);

    // Player position (clamped to valid map coordinates)
    int playerX = (int)fmax(0, fmin(mapState.mapWidth - 1, playerState.player.x));
    int playerY = (int)fmax(0, fmin(mapState.mapHeight - 1, playerState.player.y));

    // Calculate window viewport start position centered on player
    int startX = playerX - windowWidth / 2;
    int startY = playerY - windowHeight / 2;

    // Clamp viewport to map boundaries
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (startX + windowWidth > mapState.mapWidth) startX = mapState.mapWidth - windowWidth;
    if (startY + windowHeight > mapState.mapHeight) startY = mapState.mapHeight - windowHeight;
    // Double-check for small maps
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    // === Render background (undiscovered areas) ===
    for (int y = 0; y < windowHeight; y++) {
        float deformation = 1.0f + y * increment;
        for (int x = 0; x < windowWidth; x++) {
            SDL_SetRenderDrawColor(renderer, MINIMAP_BG_COLOR.r, MINIMAP_BG_COLOR.g, 
                                 MINIMAP_BG_COLOR.b, MINIMAP_BG_COLOR.a);
            
            SDL_Rect mapRect = {
                (int)((screenWidth - drawSize * deformation * windowWidth) / 2) + (int)(x * drawSize * deformation),
                (screenHeight - drawSize * windowHeight - MINIMAP_MARGIN_BOTTOM) + y * drawSize,
                (int)(drawSize * deformation) + 1,
                drawSize
            };
            SDL_RenderFillRect(renderer, &mapRect);
        }
    }

    // === Render discovered map areas ===
    for (int y = 0; y < windowHeight; y++) {
        float deformation = 1.0f + y * increment;
        for (int x = 0; x < windowWidth; x++) {
            int mapX = startX + x;
            int mapY = startY + y;

            // Bounds check for map coordinates
            if (mapX >= 0 && mapX < mapState.mapWidth && 
                mapY >= 0 && mapY < mapState.mapHeight &&
                mapState.mapDiscovered[mapY][mapX] == 1) {
                
                // Set color based on wall type
                Color wallColor;
                switch (mapState.map[mapY][mapX]) {
                    case 1:  wallColor = MINIMAP_WALL1_COLOR; break;
                    case 2:  wallColor = MINIMAP_WALL2_COLOR; break;
                    case 3:  wallColor = MINIMAP_WALL3_COLOR; break;
                    case 4:  wallColor = MINIMAP_WALL4_COLOR; break;
                    case 5:  wallColor = MINIMAP_WALL5_COLOR; break;
                    default: wallColor = MINIMAP_FLOOR_COLOR; break;
                }
                
                SDL_SetRenderDrawColor(renderer, wallColor.r, wallColor.g, wallColor.b, wallColor.a);

                SDL_Rect block = {
                    (int)((screenWidth - drawSize * deformation * windowWidth) / 2) + (int)(x * drawSize * deformation),
                    (screenHeight - drawSize * windowHeight - MINIMAP_MARGIN_BOTTOM) + y * drawSize,
                    (int)(drawSize * deformation) + 1,
                    drawSize
                };
                SDL_RenderFillRect(renderer, &block);
            }
        }
    }

    // === Render player position ===
    float playerDeformation = 1.0f + (playerY - startY) * increment;

    SDL_SetRenderDrawColor(renderer, MINIMAP_PLAYER_COLOR.r, MINIMAP_PLAYER_COLOR.g, 
                          MINIMAP_PLAYER_COLOR.b, MINIMAP_PLAYER_COLOR.a);
    
    SDL_Rect playerRect = {
        (int)((screenWidth - drawSize * playerDeformation * windowWidth) / 2) + 
            (int)((playerX - startX) * drawSize * playerDeformation),
        (screenHeight - drawSize * windowHeight - MINIMAP_MARGIN_BOTTOM) + (playerY - startY) * drawSize,
        MINIMAP_PLAYER_SIZE, 
        MINIMAP_PLAYER_SIZE
    };
    SDL_RenderFillRect(renderer, &playerRect);
}