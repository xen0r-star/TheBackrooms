#include "entity.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

void render_sprites(GameState*state, Sprite *sprites, int numSprites, double *zBuffer)
{
    double posX = state->playerState.player.x;
    double posY = state->playerState.player.y;

    float dirAngle = state->playerState.player.angle * (PI / 180.0);
    float dirX = cos(dirAngle);
    float dirY = sin(dirAngle);

    float planeX = -dirY * tanf((state->settings.fov / 2.0f) * PI / 180.0f);
    float planeY = dirX * tanf((state->settings.fov / 2.0f) * PI / 180.0f);

    const int w = state->app.screenWidth;
    const int h = state->app.screenHeight;

    int *spriteOrder = malloc(numSprites * sizeof(int));
    double *spriteDistance = malloc(numSprites * sizeof(double));

    for (int i = 0; i < numSprites; i++) {
        spriteOrder[i] = i;
        spriteDistance[i] = (posX - sprites[i].x) * (posX - sprites[i].x) + 
                            (posY - sprites[i].y) * (posY - sprites[i].y);
    }

    // Sort sprites by distance
    for (int i = 0; i < numSprites - 1; i++) {
        for (int j = i + 1; j < numSprites; j++) {
            if (spriteDistance[i] < spriteDistance[j]) {
                double tmpDist = spriteDistance[i];
                spriteDistance[i] = spriteDistance[j];
                spriteDistance[j] = tmpDist;
                int tmpOrder = spriteOrder[i];
                spriteOrder[i] = spriteOrder[j];
                spriteOrder[j] = tmpOrder;
            }
        }
    }


    for (int i = 0; i < numSprites; i++) {
        float vDiv = sprites[spriteOrder[i]].scaleY;
        float uDiv = sprites[spriteOrder[i]].scaleX;
        float vMove = sprites[spriteOrder[i]].transform.moveY;
        float transparency = sprites[spriteOrder[i]].transform.transparency;


        double spriteX = sprites[spriteOrder[i]].x - posX;
        double spriteY = sprites[spriteOrder[i]].y - posY;

        double invDet = 1.0 / (planeX * dirY - dirX * planeY);

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        int spriteScreenX = (int)((w / 2) * (1 + transformX / transformY));
        int vMoveScreen = (int)(-vMove / transformY);

        if (vDiv == 0) vDiv = 1; 
        int spriteHeight = abs((int)(h / transformY)) / (1.0f / vDiv);
        int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
        if (drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
        if (drawEndY >= h) drawEndY = h - 1;

        if (uDiv == 0) uDiv = 1; 
        int spriteWidth = abs((int)(h / transformY)) / (1.0f / uDiv);
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= w) drawEndX = w - 1;

        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXTURE_SIZE / spriteWidth) / 256;

            if (transformY > 0 && stripe >= 0 && stripe < w && transformY < zBuffer[stripe]) {
                for (int y = drawStartY; y < drawEndY; y++) {
                    int d = (y - vMoveScreen) * 256 - h * 128 + spriteHeight * 128;
                    int texY = ((d * TEXTURE_SIZE) / spriteHeight) / 256;

                    Uint32 pixel = state->graphics.textureBuffers[sprites[spriteOrder[i]].texture_id][TEXTURE_SIZE * texY + texX];
                    
                    Uint8 originalA = pixel & 0xFF;
                    if (originalA > 0) {
                        if (transparency >= 1.0f || transparency <= 0.0f) {
                            Uint8 r = ((pixel >> 24) & 0xFF) / 2;
                            Uint8 g = ((pixel >> 16) & 0xFF) / 2;
                            Uint8 b = ((pixel >> 8 ) & 0xFF) / 2;
                            
                            state->graphics.screenBuffers[y * w + stripe] = (0xFF << 24) | (r << 16) | (g << 8) | b;

                        } else {
                            Uint8 r = ((pixel >> 24) & 0xFF) / 2;
                            Uint8 g = ((pixel >> 16) & 0xFF) / 2;
                            Uint8 b = ((pixel >> 8 ) & 0xFF) / 2;
    
                            // Lire la couleur du fond
                            Uint32 bgPixel = state->graphics.screenBuffers[y * w + stripe];
                            Uint8 bgR = (bgPixel >> 16) & 0xFF;
                            Uint8 bgG = (bgPixel >> 8)  & 0xFF;
                            Uint8 bgB = bgPixel & 0xFF;
    
                            // Mélange RGB avec fond
                            Uint8 finalR = (Uint8)(r * transparency + bgR * (1.0f - transparency));
                            Uint8 finalG = (Uint8)(g * transparency + bgG * (1.0f - transparency));
                            Uint8 finalB = (Uint8)(b * transparency + bgB * (1.0f - transparency));
    
                            state->graphics.screenBuffers[y * w + stripe] = (0xFF << 24) | (finalR << 16) | (finalG << 8) | finalB;
                        }
                    }
                }
            }
        }
    }

    free(spriteOrder);
    free(spriteDistance);
}
