#include "render.h"

void renderTexturedWall(int x, float rayAngle, float distance, int wallType, int wallSide, int wallHeight, int drawStart, int drawEnd) {
    double wallX;
    if (wallSide == 0)
        wallX = player.y + distance * sin(rayAngle);
    else
        wallX = player.x + distance * cos(rayAngle);
    wallX -= floor((wallX));


    int texX = (int)(wallX * (double)textureSize);
    if (wallSide == 0 && cos(rayAngle) > 0)
        texX = textureSize - texX - 1;
    if (wallSide == 1 && sin(rayAngle) < 0)
        texX = textureSize - texX - 1;

    double step = 1.0 * textureSize / wallHeight;
    double texPos = (drawStart - SCREEN_HEIGHT / 2 + wallHeight / 2) * step;

    for (int y = drawStart; y < drawEnd; y++) {
        int texY = (int)texPos & (textureSize - 1);
        texPos += step;

        Uint32 pixel = textureBuffers[wallType - 1][textureSize * texY + texX];
        Uint8 r = ((pixel >> 24) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 g = ((pixel >> 16) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 b = ((pixel >> 8 ) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 a = pixel & 0xFF;

        float fogFactor = fmin(distance / 60.0, 0.5);

        r = (Uint8)((1 - fogFactor) * r + fogFactor * 170);
        g = (Uint8)((1 - fogFactor) * g + fogFactor * 170);
        b = (Uint8)((1 - fogFactor) * b + fogFactor * 170);

        screenBuffers[y * SCREEN_WIDTH + x] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

void renderTexturedFloor(int y, float floorX, float floorY, float floorStepX, float floorStepY) {
    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        int texX = (int)(textureSize * (floorX - cellX)) & (textureSize - 1);
        int texY = (int)(textureSize * (floorY - cellY)) & (textureSize - 1);

        floorX += floorStepX;
        floorY += floorStepY;
        

        Uint32 pixel;
        Uint8 r, g, b, a;

        pixel = textureBuffers[numberTextures - 1][textureSize * texY + texX];
        r = ((pixel >> 24) & 0xFF) / 2;
        g = ((pixel >> 16) & 0xFF) / 2;
        b = ((pixel >> 8 ) & 0xFF) / 2;
        a = pixel & 0xFF;

        screenBuffers[y * SCREEN_WIDTH + x] = (a << 24) | (r << 16) | (g << 8) | b;


        pixel = textureBuffers[numberTextures - 2][textureSize * texY + texX];
        r = ((pixel >> 24) & 0xFF) / 2;
        g = ((pixel >> 16) & 0xFF) / 2;
        b = ((pixel >> 8 ) & 0xFF) / 2;
        a = pixel & 0xFF;

        screenBuffers[(SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH + x] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

void renderColoredWall(int x, float distance, int wallType, int wallSide, int wallHeight, int drawStart) {
    Color color;
    switch (wallType) {
        case 1: color = (Color){184, 181,  55, 255}; break;
        case 2: color = (Color){184, 181,  55, 255}; break;
        case 3: color = (Color){  0, 255,   0, 255}; break;
        case 4: color = (Color){  0,   0, 255, 255}; break;
        case 5: color = (Color){255,   0, 255, 255}; break;
        default:color = (Color){  0,   0,   0, 255}; break;
    }

    if (wallSide == 1) {
        color.r /=2;
        color.g /=2;
        color.b /=2;
    }

    float fogFactor = fmin(distance / 60.0, 0.5);

    color.r = (1 - fogFactor) * color.r + fogFactor * 170;
    color.g = (1 - fogFactor) * color.g + fogFactor * 170;
    color.b = (1 - fogFactor) * color.b + fogFactor * 170;

    // Dessiner le mur
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        if (y < drawStart) {
            screenBuffers[y * SCREEN_WIDTH + x] = 0xFF474112;
        } else if (y > drawStart + wallHeight) {
            screenBuffers[y * SCREEN_WIDTH + x] = 0xFF524B1C;
        } else {
            screenBuffers[y * SCREEN_WIDTH + x] = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
        }
    }
}





// renderScene: Fonction pour dessiner la scène
void renderScene() {
    // Dessin des colonnes de murs
    if (showTextures) {
        for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; y++) { // sol et plafond
            float angleRadians0 = (player.angle - (FOV / 2.0)) * (M_PI / 180.0);
            float angleRadians1 = (player.angle + (FOV / 2.0)) * (M_PI / 180.0);

            float rayDirX0 = cos(angleRadians0);
            float rayDirY0 = sin(angleRadians0);
            float rayDirX1 = cos(angleRadians1);
            float rayDirY1 = sin(angleRadians1);


            int p = y - SCREEN_HEIGHT / 2;
            float posZ = 0.5 * SCREEN_HEIGHT;
            float rowDistance = posZ / p;

            float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
            float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

            float floorX = player.x + rowDistance * rayDirX0;
            float floorY = player.y + rowDistance * rayDirY0;

            renderTexturedFloor(y, floorX, floorY, floorStepX, floorStepY);
        }

        for (int x = 0; x < SCREEN_WIDTH; x++) { // murs
            float rayAngle = ((player.angle - (FOV / 2.0)) + ((float)x / (float)SCREEN_WIDTH) * FOV) * (M_PI / 180); // Angle du rayon
            float distance;
            int wallType, wallSide;

            castRay(rayAngle, &distance, &wallType, &wallSide);   // Convertir en radians

            int wallHeight = (int)(SCREEN_HEIGHT / distance);         // Hauteur du mur basée sur la distance
            int drawStart = (SCREEN_HEIGHT - wallHeight) / 2;         // Point de départ du mur
            int drawEnd = drawStart + wallHeight;                     // Point de fin du mur

            if (drawStart < 0) drawStart = 0;                         
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            renderTexturedWall(x, rayAngle, distance, wallType, wallSide, wallHeight, drawStart, drawEnd);
        }
    } else {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            float rayAngle = ((player.angle - (FOV / 2.0)) + ((float)x / (float)SCREEN_WIDTH) * FOV) * (M_PI / 180); // Angle du rayon
            float distance;
            int wallType, wallSide;

            castRay(rayAngle, &distance, &wallType, &wallSide);   // Convertir en radians

            int wallHeight = (int)(SCREEN_HEIGHT / distance);         // Hauteur du mur basée sur la distance
            int drawStart = (SCREEN_HEIGHT - wallHeight) / 2;         // Point de départ du mur
            int drawEnd = drawStart + wallHeight;                     // Point de fin du mur

            if (drawStart < 0) drawStart = 0;                         
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            renderColoredWall(x, distance, wallType, wallSide, wallHeight, drawStart);
        }
    }

    SDL_UpdateTexture(screenBuffersTexture, NULL, screenBuffers, SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderCopy(renderer, screenBuffersTexture, NULL, NULL);
}