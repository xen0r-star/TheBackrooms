#include "map.h"

void spawnPlayerFromMap(PlayerState *playerState, MapState *mapState) {
    for (int y = 0; y < mapState->mapHeight; y++) {
        for (int x = 0; x < mapState->mapWidth; x++) {
            if (mapState->map[y][x] == 9) {
                playerState->player.x = x + 0.5f;  // Centré dans la case
                playerState->player.y = y + 0.5f;
                playerState->player.angle = 0.0f;  // Orientation par défaut
                mapState->map[y][x] = 0;           // Réinitialise la case
                return;
            }
        }
    }

    playerState->player.x = 1.5f;
    playerState->player.y = 1.5f;
    playerState->player.angle = 0.0f;
    
    printf("Aucun point de spawn trouvé, position par défaut utilisée.\n");
}