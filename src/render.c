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

        r = (Uint8)((1 - fogFactor) * r + fogFactor * 0);
        g = (Uint8)((1 - fogFactor) * g + fogFactor * 0);
        b = (Uint8)((1 - fogFactor) * b + fogFactor * 0);

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



void glitchEffect(int speed) {
    srand(clock() / (1000 / speed));

    // 1. Lignes de balayage (scan lines)
    for (int y = 0; y < SCREEN_HEIGHT; y += 2) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * SCREEN_WIDTH + x;
            screenBuffers[index] = (50 << 24) | (0 << 16) | (0 << 8) | 0;
        }
    }

    // 2. Distorsions de ligne aléatoires
    for (int y = 0; y < SCREEN_HEIGHT; y += 15) { // Bandes de 15 pixels de hauteur
        int shift = (rand() % 10) - 5; // Décalage aléatoire entre -5 et +5 pixels

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int index = y * SCREEN_WIDTH + x;
            int shiftedX = x + shift;

            if (shiftedX >= 0 && shiftedX < SCREEN_WIDTH) {
                screenBuffers[index] = screenBuffers[y * SCREEN_WIDTH + shiftedX];
            }
        }
    }

    // 3. Bruit de couleur (artefacts)
    // for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT / 50; i++) { // Quelques pixels aléatoires
    //     int randomX = rand() % SCREEN_WIDTH;
    //     int randomY = rand() % SCREEN_HEIGHT;
    //     int index = randomY * SCREEN_WIDTH + randomX;
        
    //     Uint8 r = rand() % 256;
    //     Uint8 g = rand() % 256;
    //     Uint8 b = rand() % 256;
    //     screenBuffers[index] = (255 << 24) | (r << 16) | (g << 8) | b;
    // }

    // 4. Déformation de couleur
    // for (int y = 0; y < SCREEN_HEIGHT; y++) {
    //     for (int x = 0; x < SCREEN_WIDTH; x++) {
    //         int index = y * SCREEN_WIDTH + x;

    //         Uint8 r = (screenBuffers[index] >> 16) & 0xFF;
    //         Uint8 g = (screenBuffers[index] >> 8) & 0xFF;
    //         Uint8 b = screenBuffers[index] & 0xFF;

    //         int redX = x + 1;  // Décalage de rouge vers la droite
    //         int blueX = x - 1; // Décalage de bleu vers la gauche

    //         //  ? redX : blueX;
    //         if (rand() % 18 == 3 && redX < SCREEN_WIDTH) {
    //             screenBuffers[y * SCREEN_WIDTH + redX] = (255 << 24) | (r << 16) | (0 << 8) | 0;
    //         } else if (rand() % 18 == 6 && blueX >= 0) {
    //             screenBuffers[y * SCREEN_WIDTH + blueX] |= (255 << 24) | (0 << 16) | (0 << 8) | 70;
    //         }
    //     }
    // }
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

    // glitchEffect(1);

    SDL_UpdateTexture(screenBuffersTexture, NULL, screenBuffers, SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderCopy(renderer, screenBuffersTexture, NULL, NULL);
}