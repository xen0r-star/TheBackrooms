#include "map.h"


void spawnPlayerFromMap(PlayerState *playerState, MapState *mapState) {
    // Validate input parameters
    if (!playerState || !mapState || !mapState->map) {
        fprintf(stderr, "Error: Invalid parameters for player spawn\n");
        return;
    }

    // Search for spawn point in the map
    for (int y = 0; y < mapState->mapHeight; y++) {
        for (int x = 0; x < mapState->mapWidth; x++) {
            if (mapState->map[y][x] == TILE_SPAWN) {
                // Position player at center of spawn tile
                playerState->player.x = x + 0.5f;
                playerState->player.y = y + 0.5f;
                playerState->player.angle = 0.0f;  // Default orientation
                
                // Clear spawn tile to prevent reuse
                mapState->map[y][x] = TILE_EMPTY;
                
                printf("Player spawned at map coordinates (%.1f, %.1f)\n", 
                       playerState->player.x, playerState->player.y);
                return;
            }
        }
    }

    // Fallback: use default spawn position if no spawn point found
    playerState->player.x = 1.5f;
    playerState->player.y = 1.5f;
    playerState->player.angle = 0.0f;
    
    printf("Warning: No spawn point found in map, using default position (1.5, 1.5)\n");
}