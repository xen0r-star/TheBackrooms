#include "render.h"

void renderTexturedWall(GameState *state, int x, float rayAngle, float distance, int wallType, int wallSide, int wallHeight, int drawStart, int drawEnd) {
    double wallX;
    if (wallSide == 0)
        wallX = state->playerState.player.y + distance * sin(rayAngle);
    else
        wallX = state->playerState.player.x + distance * cos(rayAngle);
    wallX -= floor((wallX));


    int texX = (int)(wallX * (double)TEXTURE_SIZE);
    if (wallSide == 0 && cos(rayAngle) > 0)
        texX = TEXTURE_SIZE - texX - 1;
    if (wallSide == 1 && sin(rayAngle) < 0)
        texX = TEXTURE_SIZE - texX - 1;

    double step = 1.0 * TEXTURE_SIZE / wallHeight;
    double texPos = (drawStart - state->app.screenHeight / 2 + wallHeight / 2) * step;

    for (int y = drawStart; y < drawEnd; y++) {
        int texY = (int)texPos & (TEXTURE_SIZE - 1);
        texPos += step;

        Uint32 pixel = state->graphics.textureBuffers[wallType - 1][TEXTURE_SIZE * texY + texX];
        Uint8 r = ((pixel >> 24) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 g = ((pixel >> 16) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 b = ((pixel >> 8 ) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 a = pixel & 0xFF;

        float fogFactor = fmin(distance / 60.0, 0.5);

        r = (Uint8)((1 - fogFactor) * r + fogFactor * 0);
        g = (Uint8)((1 - fogFactor) * g + fogFactor * 0);
        b = (Uint8)((1 - fogFactor) * b + fogFactor * 0);

        state->graphics.screenBuffers[y * state->app.screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

void renderTexturedFloor(GameState *state, int y, float floorX, float floorY, float floorStepX, float floorStepY) {
    for (int x = 0; x < state->app.screenWidth; ++x) {
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        int texX = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
        int texY = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);

        floorX += floorStepX;
        floorY += floorStepY;
        

        Uint32 pixel;
        Uint8 r, g, b, a;

        pixel = state->graphics.textureBuffers[NUMBER_TEXTURES - 1][TEXTURE_SIZE * texY + texX];
        r = ((pixel >> 24) & 0xFF) / 2;
        g = ((pixel >> 16) & 0xFF) / 2;
        b = ((pixel >> 8 ) & 0xFF) / 2;
        a = pixel & 0xFF;

        state->graphics.screenBuffers[y * state->app.screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;


        pixel = state->graphics.textureBuffers[NUMBER_TEXTURES - 2][TEXTURE_SIZE * texY + texX];
        r = ((pixel >> 24) & 0xFF) / 2;
        g = ((pixel >> 16) & 0xFF) / 2;
        b = ((pixel >> 8 ) & 0xFF) / 2;
        a = pixel & 0xFF;

        state->graphics.screenBuffers[(state->app.screenHeight - y - 1) * state->app.screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

void renderColoredWall(GameState *state, int x, float distance, int wallType, int wallSide, int wallHeight, int drawStart) {
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
    for (int y = 0; y < state->app.screenHeight; y++) {
        if (y < drawStart) {
            state->graphics.screenBuffers[y * state->app.screenWidth + x] = 0xFF474112;
        } else if (y > drawStart + wallHeight) {
            state->graphics.screenBuffers[y * state->app.screenWidth + x] = 0xFF524B1C;
        } else {
            state->graphics.screenBuffers[y * state->app.screenWidth + x] = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
        }
    }
}



void glitchEffect(GameState *state, int speed) {
    srand(clock() / (1000 / speed));

    // 1. Lignes de balayage (scan lines)
    for (int y = 0; y < state->app.screenHeight; y += 2) {
        for (int x = 0; x < state->app.screenWidth; x++) {
            int index = y * state->app.screenWidth + x;
            state->graphics.screenBuffers[index] = (50 << 24) | (0 << 16) | (0 << 8) | 0;
        }
    }

    // 2. Distorsions de ligne aléatoires
    for (int y = 0; y < state->app.screenHeight; y += 15) { // Bandes de 15 pixels de hauteur
        int shift = (rand() % 10) - 5; // Décalage aléatoire entre -5 et +5 pixels

        for (int x = 0; x < state->app.screenWidth; x++) {
            int index = y * state->app.screenWidth + x;
            int shiftedX = x + shift;

            if (shiftedX >= 0 && shiftedX < state->app.screenWidth) {
                state->graphics.screenBuffers[index] = state->graphics.screenBuffers[y * state->app.screenWidth + shiftedX];
            }
        }
    }

    // 3. Bruit de couleur (artefacts)
    // for (int i = 0; i < screenWidth * screenHeight / 50; i++) { // Quelques pixels aléatoires
    //     int randomX = rand() % screenWidth;
    //     int randomY = rand() % screenHeight;
    //     int index = randomY * screenWidth + randomX;
        
    //     Uint8 r = rand() % 256;
    //     Uint8 g = rand() % 256;
    //     Uint8 b = rand() % 256;
    //     screenBuffers[index] = (255 << 24) | (r << 16) | (g << 8) | b;
    // }

    // 4. Déformation de couleur
    // for (int y = 0; y < screenHeight; y++) {
    //     for (int x = 0; x < screenWidth; x++) {
    //         int index = y * screenWidth + x;

    //         Uint8 r = (screenBuffers[index] >> 16) & 0xFF;
    //         Uint8 g = (screenBuffers[index] >> 8) & 0xFF;
    //         Uint8 b = screenBuffers[index] & 0xFF;

    //         int redX = x + 1;  // Décalage de rouge vers la droite
    //         int blueX = x - 1; // Décalage de bleu vers la gauche

    //         //  ? redX : blueX;
    //         if (rand() % 18 == 3 && redX < screenWidth) {
    //             screenBuffers[y * screenWidth + redX] = (255 << 24) | (r << 16) | (0 << 8) | 0;
    //         } else if (rand() % 18 == 6 && blueX >= 0) {
    //             screenBuffers[y * screenWidth + blueX] |= (255 << 24) | (0 << 16) | (0 << 8) | 70;
    //         }
    //     }
    // }
}


// renderScene: Fonction pour dessiner la scène
void renderScene(GameState *state) {
    // Dessin des colonnes de murs
    if (state->playerState.showTextures) {
        for (int y = state->app.screenHeight / 2 + 1; y < state->app.screenHeight; y++) { // sol et plafond
            float angleRadians0 = (state->playerState.player.angle - (state->settings.fov / 2.0)) * (M_PI / 180.0);
            float angleRadians1 = (state->playerState.player.angle + (state->settings.fov / 2.0)) * (M_PI / 180.0);

            float rayDirX0 = cos(angleRadians0);
            float rayDirY0 = sin(angleRadians0);
            float rayDirX1 = cos(angleRadians1);
            float rayDirY1 = sin(angleRadians1);


            int p = y - state->app.screenHeight / 2;
            float posZ = 0.5 * state->app.screenHeight;
            float rowDistance = posZ / p;

            float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / state->app.screenWidth;
            float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / state->app.screenWidth;

            float floorX = state->playerState.player.x + rowDistance * rayDirX0;
            float floorY = state->playerState.player.y + rowDistance * rayDirY0;

            renderTexturedFloor(state, y, floorX, floorY, floorStepX, floorStepY);
        }

        for (int x = 0; x < state->app.screenWidth; x++) { // murs
            float rayAngle = ((state->playerState.player.angle - (state->settings.fov / 2.0)) + ((float)x / (float)state->app.screenWidth) * state->settings.fov) * (M_PI / 180); // Angle du rayon
            float distance;
            int wallType, wallSide;

            castRay(state, rayAngle, &distance, &wallType, &wallSide);   // Convertir en radians

            int wallHeight = (int)(state->app.screenHeight / distance);         // Hauteur du mur basée sur la distance
            int drawStart = (state->app.screenHeight - wallHeight) / 2;         // Point de départ du mur
            int drawEnd = drawStart + wallHeight;                     // Point de fin du mur

            if (drawStart < 0) drawStart = 0;                         
            if (drawEnd >= state->app.screenHeight) drawEnd = state->app.screenHeight - 1;

            renderTexturedWall(state, x, rayAngle, distance, wallType, wallSide, wallHeight, drawStart, drawEnd);
        }
    } else {
        for (int x = 0; x < state->app.screenWidth; x++) {
            float rayAngle = ((state->playerState.player.angle - (state->settings.fov / 2.0)) + ((float)x / (float)state->app.screenWidth) * state->settings.fov) * (M_PI / 180); // Angle du rayon
            float distance;
            int wallType, wallSide;

            castRay(state, rayAngle, &distance, &wallType, &wallSide);   // Convertir en radians

            int wallHeight = (int)(state->app.screenHeight / distance);         // Hauteur du mur basée sur la distance
            int drawStart = (state->app.screenHeight - wallHeight) / 2;         // Point de départ du mur
            int drawEnd = drawStart + wallHeight;                     // Point de fin du mur

            if (drawStart < 0) drawStart = 0;                         
            if (drawEnd >= state->app.screenHeight) drawEnd = state->app.screenHeight - 1;

            renderColoredWall(state, x, distance, wallType, wallSide, wallHeight, drawStart);
        }
    }

    // glitchEffect(1);

    SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, state->graphics.screenBuffers, state->app.screenWidth * sizeof(Uint32));
    SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);
}