#include "castRay.h"

// castRay: Fonction qui trace un rayon => retourne la distance jusqu'au mur et le type de mur
int castRay(float angle, float *distance, int *wallType, int *wallSide) {
    float x = player.x;
    float y = player.y;

    float rayDirX = cos(angle);
    float rayDirY = sin(angle);

    float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

    float stepX, stepY;
    float sideDistX, sideDistY;
    int side;

    int mapX = (int)x;
    int mapY = (int)y;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (x - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - x) * deltaDistX;
    }

    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (y - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - y) * deltaDistY;
    }

    while (1) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        if (map[mapY][mapX] > 0) {
            if (side == 0) *distance = (sideDistX - deltaDistX);
            else           *distance = (sideDistY - deltaDistY);

            mapDiscovered[mapY][mapX] = 1;

            *wallType = map[mapY][mapX];
            *wallSide = side;

            return 0;
        } else {
            mapDiscovered[mapY][mapX] = 1;
        }
    }
}
