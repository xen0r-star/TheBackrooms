#include "graphics.h"

#define PI 3.14159265358979323846

void renderTexturedWall(const AppState appState, const PlayerState playerState, GraphicsBuffers *graphicsBuffers, int x, float rayAngle, float distance, int wallType, int wallSide, int wallHeight, int drawStart, int drawEnd);
void renderTexturedFloor(const AppState appState, GraphicsBuffers *graphicsBuffers, int y, float floorX, float floorY, float floorStepX, float floorStepY);
void renderColoredWall(const AppState appState, GraphicsBuffers *graphicsBuffers, int x, float distance, int wallType, int wallSide, int wallHeight, int drawStart);


// renderScene: Fonction pour dessiner la scène
void renderScene(GameState *state) {
    // Dessin des colonnes de murs

    if (state->playerState.showTextures) {
        for (int y = state->app.screenHeight / 2 + 1; y < state->app.screenHeight; y++) { // sol et plafond
            float angleRadians0 = (state->playerState.player.angle - (state->settings.fov / 2.0)) * (PI / 180.0);
            float angleRadians1 = (state->playerState.player.angle + (state->settings.fov / 2.0)) * (PI / 180.0);

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

            renderTexturedFloor(state->app, &state->graphics, y, floorX, floorY, floorStepX, floorStepY);
        }

        for (int x = 0; x < state->app.screenWidth; x++) { // murs
            float rayAngle = ((state->playerState.player.angle - (state->settings.fov / 2.0)) 
                             + ((float)x / (float)state->app.screenWidth) * state->settings.fov) 
                             * (PI / 180); // Angle du rayon
            float distance;
            int wallType, wallSide;

            castRay(state->playerState, &state->mapState, rayAngle, &distance, &wallType, &wallSide);   // Convertir en radians

            int wallHeight = (int)(state->app.screenHeight / distance);         // Hauteur du mur basée sur la distance
            int drawStart = (state->app.screenHeight - wallHeight) / 2;         // Point de départ du mur
            int drawEnd = drawStart + wallHeight;                               // Point de fin du mur

            if (drawStart < 0) drawStart = 0;                         
            if (drawEnd >= state->app.screenHeight) drawEnd = state->app.screenHeight - 1;

            // glitchEffect(state->app, &state->graphics, 1);


            if (wallType == 9) 
                wallType = 0;

            renderTexturedWall(state->app, state->playerState, &state->graphics, x, rayAngle, distance, wallType, wallSide, wallHeight, drawStart, drawEnd);
        }
    } else {
        for (int x = 0; x < state->app.screenWidth; x++) {
            float rayAngle = ((state->playerState.player.angle - (state->settings.fov / 2.0)) 
                             + ((float)x / (float)state->app.screenWidth) * state->settings.fov)
                             * (PI / 180);  // Angle du rayon
            float distance;
            int wallType, wallSide;

            castRay(state->playerState, &state->mapState, rayAngle, &distance, &wallType, &wallSide);   // Convertir en radians

            int wallHeight = (int)(state->app.screenHeight / distance);         // Hauteur du mur basée sur la distance
            int drawStart = (state->app.screenHeight - wallHeight) / 2;         // Point de départ du mur
            int drawEnd = drawStart + wallHeight;                               // Point de fin du mur

            if (drawStart < 0) drawStart = 0;                         
            if (drawEnd >= state->app.screenHeight) drawEnd = state->app.screenHeight - 1;

            renderColoredWall(state->app, &state->graphics, x, distance, wallType, wallSide, wallHeight, drawStart);
        }
    }

    SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, state->graphics.screenBuffers, state->app.screenWidth * sizeof(Uint32));
    SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);
}






void renderTexturedWall(const AppState appState, const PlayerState playerState, GraphicsBuffers *graphicsBuffers, int x, float rayAngle, float distance, int wallType, int wallSide, int wallHeight, int drawStart, int drawEnd) {
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;
    
    double wallX;
    if (wallSide == 0)
        wallX = playerState.player.y + distance * sin(rayAngle);
    else
        wallX = playerState.player.x + distance * cos(rayAngle);
    wallX -= floor((wallX));


    int texX = (int)(wallX * (double)TEXTURE_SIZE);
    if (wallSide == 0 && cos(rayAngle) > 0)
        texX = TEXTURE_SIZE - texX - 1;
    if (wallSide == 1 && sin(rayAngle) < 0)
        texX = TEXTURE_SIZE - texX - 1;

    double step = 1.0 * TEXTURE_SIZE / wallHeight;
    double texPos = (drawStart - screenHeight / 2 + wallHeight / 2) * step;

    for (int y = drawStart; y < drawEnd; y++) {
        int texY = (int)texPos & (TEXTURE_SIZE - 1);
        texPos += step;

        Uint32 pixel = graphicsBuffers->textureBuffers[wallType - 1][TEXTURE_SIZE * texY + texX];
        Uint8 r = ((pixel >> 24) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 g = ((pixel >> 16) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 b = ((pixel >> 8 ) & 0xFF) / (wallSide == 1 ? 2 : 1);
        Uint8 a = pixel & 0xFF;

        float fogFactor = fmin(distance / 30.0, 0.5);
        fogFactor = powf(fogFactor, 2.0f);

        r = (Uint8)((1 - fogFactor) * r + fogFactor * 0);
        g = (Uint8)((1 - fogFactor) * g + fogFactor * 0);
        b = (Uint8)((1 - fogFactor) * b + fogFactor * 0);

        graphicsBuffers->screenBuffers[y * screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}



void renderTexturedFloor(const AppState appState, GraphicsBuffers *graphicsBuffers, int y, float floorX, float floorY, float floorStepX, float floorStepY) {
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;
    
    for (int x = 0; x < screenWidth; ++x) {
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        int texX = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
        int texY = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);

        floorX += floorStepX;
        floorY += floorStepY;
        

        Uint32 pixel;
        Uint8 r, g, b, a;

        pixel = graphicsBuffers->textureBuffers[NUMBER_TEXTURES - 1][TEXTURE_SIZE * texY + texX];
        r = ((pixel >> 24) & 0xFF) / 2;
        g = ((pixel >> 16) & 0xFF) / 2;
        b = ((pixel >> 8 ) & 0xFF) / 2;
        a = pixel & 0xFF;

        graphicsBuffers->screenBuffers[y * screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;


        pixel = graphicsBuffers->textureBuffers[NUMBER_TEXTURES - 2][TEXTURE_SIZE * texY + texX];
        r = ((pixel >> 24) & 0xFF) / 2;
        g = ((pixel >> 16) & 0xFF) / 2;
        b = ((pixel >> 8 ) & 0xFF) / 2;
        a = pixel & 0xFF;

        graphicsBuffers->screenBuffers[(screenHeight - y - 1) * screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}




void renderColoredWall(const AppState appState, GraphicsBuffers *graphicsBuffers, int x, float distance, int wallType, int wallSide, int wallHeight, int drawStart) {
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

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
    for (int y = 0; y < screenHeight; y++) {
        if (y < drawStart) {
            graphicsBuffers->screenBuffers[y * screenWidth + x] = 0xFF474112;
        } else if (y > drawStart + wallHeight) {
            graphicsBuffers->screenBuffers[y * screenWidth + x] = 0xFF524B1C;
        } else {
            graphicsBuffers->screenBuffers[y * screenWidth + x] = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
        }
    }
}