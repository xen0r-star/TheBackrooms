#include "core.h"


int castRay(const PlayerState playerState, MapState *mapState, float rayDirX, float rayDirY, float *distance, int *wallType, int *wallSide) {
    // Validate input parameters
    if (!mapState || !distance || !wallType || !wallSide) {
        return -1; // Invalid parameters
    }

    float x = playerState.player.x;
    float y = playerState.player.y;

    // Calculate delta distances (length of ray from current position to x or y side)
    float deltaDistX = (rayDirX == 0) ? 1e30f : fabsf(1.0f / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30f : fabsf(1.0f / rayDirY);

    float stepX, stepY;
    float sideDistX, sideDistY;
    int side;

    // Current map grid position
    int mapX = (int)x;
    int mapY = (int)y;

    // Calculate step direction and initial distance to next grid line
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (x - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0f - x) * deltaDistX;
    }

    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (y - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0f - y) * deltaDistY;
    }

    // Add bounds checking to prevent infinite loops and array access violations
    const int MAX_RAY_STEPS = mapState->mapWidth + mapState->mapHeight;
    int stepCount = 0;

    // DDA (Digital Differential Analyzer) algorithm
    while (stepCount < MAX_RAY_STEPS) {
        // Jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0; // Hit X-side of wall
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1; // Hit Y-side of wall
        }

        stepCount++;

        // Check for map bounds to prevent buffer overflow
        if (mapX < 0 || mapX >= mapState->mapWidth || mapY < 0 || mapY >= mapState->mapHeight) {
            // Hit map boundary, treat as wall
            *distance = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
            *wallType = 1; // Default wall type for boundaries
            *wallSide = side;
            return 0;
        }

        // Mark this position as discovered (for minimap/fog of war)
        mapState->mapDiscovered[mapY][mapX] = 1;

        // Check if we hit a wall
        if (mapState->map[mapY][mapX] > 0) {
            // Calculate perpendicular distance to avoid fisheye effect
            if (side == 0) {
                *distance = (sideDistX - deltaDistX);
            } else {
                *distance = (sideDistY - deltaDistY);
            }

            *wallType = mapState->map[mapY][mapX];
            *wallSide = side;

            return 0; // Success
        }
    }

    // Ray exceeded maximum steps (should not happen with proper map design)
    *distance = 1000.0f; // Large distance
    *wallType = 1;
    *wallSide = 0;
    return -2; // Error: ray exceeded maximum steps
}
