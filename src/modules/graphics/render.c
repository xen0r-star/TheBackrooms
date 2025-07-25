#include "graphics.h"

#define PI 3.14159265358979323846



// renderScene: Fonction pour dessiner la scène
void renderScene(GameState *state)
{
    // Dessin des colonnes de murs
    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;

    // Angle centre caméra en radians
    float dirAngle = state->playerState.player.angle * (PI / 180.0);
    float dirX = cos(dirAngle);
    float dirY = sin(dirAngle);

    // Plan de caméra (perpendiculaire à direction)
    float planeX = -dirY * tanf((state->settings.fov / 2.0f) * PI / 180.0f);
    float planeY = dirX * tanf((state->settings.fov / 2.0f) * PI / 180.0f);


    if (state->playerState.showTextures)
    {
        
        float rayDirX0 = dirX - planeX;
        float rayDirY0 = dirY - planeY;
        float rayDirX1 = dirX + planeX;
        float rayDirY1 = dirY + planeY;

        float posZ = 0.5 * screenHeight;

        // ==== DESSIN DU SOL ET DU PLAFOND ====
        for (int y = screenHeight / 2 + 1; y < screenHeight; y++)
        {
            int p = y - screenHeight / 2;
            
            float rowDistance = posZ / (float)p;
            
            float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
            float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

            float floorX = state->playerState.player.x + rowDistance * rayDirX0;
            float floorY = state->playerState.player.y + rowDistance * rayDirY0;

            for (int x = 0; x < screenWidth; ++x)
            {
                int cellX = (int)(floorX);
                int cellY = (int)(floorY);

                int texX = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
                int texY = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);

                floorX += floorStepX;
                floorY += floorStepY;

                Uint32 pixel;
                Uint8 r, g, b, a;

                pixel = state->graphics.textureBuffers[3][TEXTURE_SIZE * texY + texX];
                r = ((pixel >> 24) & 0xFF) / 2;
                g = ((pixel >> 16) & 0xFF) / 2;
                b = ((pixel >> 8) & 0xFF) / 2;
                a = pixel & 0xFF;

                state->graphics.screenBuffers[y * screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;

                pixel = state->graphics.textureBuffers[2][TEXTURE_SIZE * texY + texX];
                r = ((pixel >> 24) & 0xFF) / 2;
                g = ((pixel >> 16) & 0xFF) / 2;
                b = ((pixel >> 8) & 0xFF) / 2;
                a = pixel & 0xFF;

                state->graphics.screenBuffers[(screenHeight - y - 1) * screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
            }
        }

        // ==== DESSIN DES MURS ET ENTITER ====
        double *zBuffer = malloc(sizeof(double) * screenWidth);
        float distance;
        int wallType, wallSide;

        for (int x = 0; x < screenWidth; x++)
        {

            float cameraX = 2 * x / (double)(screenWidth) - 1;
            float rayDirX = dirX + planeX * cameraX;
            float rayDirY = dirY + planeY * cameraX;

            castRay(state->playerState, &state->mapState, rayDirX, rayDirY, &distance, &wallType, &wallSide); // Convertir en radians

            zBuffer[x] = distance;

            int wallHeight = (int)(screenHeight / distance);    // Hauteur du mur basée sur la distance
            int drawStart = -wallHeight / 2 + screenHeight / 2; // Point de départ du mur
            int drawEnd = wallHeight / 2 + screenHeight / 2;    // Point de fin du mur

            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

            // glitchEffect(state->app, &state->graphics, 1);

            if (wallType == 9) continue;

            double wallX;
            if (wallSide == 0) wallX = state->playerState.player.y + distance * rayDirY;
            else               wallX = state->playerState.player.x + distance * rayDirX;
            wallX -= floor((wallX));

            int texX = (int)(wallX * (double)TEXTURE_SIZE);
            if (wallSide == 0 && rayDirX > 0) texX = TEXTURE_SIZE - texX - 1;
            if (wallSide == 1 && rayDirY < 0) texX = TEXTURE_SIZE - texX - 1;

            double step = 1.0 * TEXTURE_SIZE / wallHeight;
            double texPos = (drawStart - screenHeight / 2 + wallHeight / 2) * step;

            for (int y = drawStart; y < drawEnd; y++)
            {
                int texY = (int)texPos & (TEXTURE_SIZE - 1);
                texPos += step;

                Uint32 pixel = state->graphics.textureBuffers[wallType - 1][TEXTURE_SIZE * texY + texX];
                Uint8 r = ((pixel >> 24) & 0xFF) / (wallSide == 1 ? 2 : 1);
                Uint8 g = ((pixel >> 16) & 0xFF) / (wallSide == 1 ? 2 : 1);
                Uint8 b = ((pixel >> 8) & 0xFF) / (wallSide == 1 ? 2 : 1);
                Uint8 a = pixel & 0xFF;

                float fogFactor = fmin(distance / 30.0, 0.5);
                fogFactor = powf(fogFactor, 2.0f);

                r = (Uint8)((1 - fogFactor) * r + fogFactor * 0);
                g = (Uint8)((1 - fogFactor) * g + fogFactor * 0);
                b = (Uint8)((1 - fogFactor) * b + fogFactor * 0);

                state->graphics.screenBuffers[y * screenWidth + x] = (a << 24) | (r << 16) | (g << 8) | b;
            }
        }

        render_sprites(state, state->entityState.sprites, state->entityState.numSprites, zBuffer);

        free(zBuffer);
    }
    else
    {   
        double *zBuffer = malloc(sizeof(double) * screenWidth);
        float distance;
        int wallType, wallSide;

        for (int x = 0; x < screenWidth; x++)
        {
            float cameraX = 2 * x / (double)(screenWidth) - 1;
            float rayDirX = dirX + planeX * cameraX;
            float rayDirY = dirY + planeY * cameraX;

            castRay(state->playerState, &state->mapState, rayDirX, rayDirY, &distance, &wallType, &wallSide); // Convertir en radians

            zBuffer[x] = distance;

            int wallHeight = (int)(screenHeight / distance); // Hauteur du mur basée sur la distance
            int drawStart = (screenHeight - wallHeight) / 2; // Point de départ du mur
            int drawEnd = drawStart + wallHeight;            // Point de fin du mur

            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

            Color color;
            switch (wallType)
            {
            case 1:
                color = (Color){184, 181, 55, 255};
                break;
            case 2:
                color = (Color){184, 181, 55, 255};
                break;
            case 3:
                color = (Color){0, 255, 0, 255};
                break;
            case 4:
                color = (Color){0, 0, 255, 255};
                break;
            case 5:
                color = (Color){255, 0, 255, 255};
                break;
            default:
                color = (Color){0, 0, 0, 255};
                break;
            }

            if (wallSide == 1)
            {
                color.r /= 2;
                color.g /= 2;
                color.b /= 2;
            }

            float fogFactor = fmin(distance / 60.0, 0.5);

            color.r = (1 - fogFactor) * color.r + fogFactor * 170;
            color.g = (1 - fogFactor) * color.g + fogFactor * 170;
            color.b = (1 - fogFactor) * color.b + fogFactor * 170;

            // Dessiner le mur
            for (int y = 0; y < screenHeight; y++)
            {
                if (y < drawStart)
                {
                    state->graphics.screenBuffers[y * screenWidth + x] = 0xFF474112;
                }
                else if (y > drawStart + wallHeight)
                {
                    state->graphics.screenBuffers[y * screenWidth + x] = 0xFF524B1C;
                }
                else
                {
                    state->graphics.screenBuffers[y * screenWidth + x] = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
                }
            }
        }

        render_sprites(state, state->entityState.sprites, state->entityState.numSprites, zBuffer);

        free(zBuffer);
    }

    SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, state->graphics.screenBuffers, screenWidth * sizeof(Uint32));
    SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);
}