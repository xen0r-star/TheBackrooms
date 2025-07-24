#include "entity.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

void render_sprites(GameState*state, Sprite *sprites, int numSprites, double *zBuffer)
{
    double posX = state->playerState.player.x;
    double posY = state->playerState.player.y;

    float angleRad = state->playerState.player.angle * (PI / 180.0);
    float dirX = cos(angleRad);
    float dirY = sin(angleRad);

    float fov = state->settings.fov * (PI / 180.0);
    float planeX = -dirY * tan(fov / 2.0);
    float planeY = dirX * tan(fov / 2.0);

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
        double spriteX = sprites[spriteOrder[i]].x - posX;
        double spriteY = sprites[spriteOrder[i]].y - posY;

        double invDet = 1.0 / (planeX * dirY - dirX * planeY);

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        int spriteScreenX = (int)((w / 2) * (1 + transformX / transformY));

        int spriteHeight = abs((int)(h / transformY));
        int drawStartY = -spriteHeight / 2 + h / 2;
        if (drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + h / 2;
        if (drawEndY >= h) drawEndY = h - 1;

        int spriteWidth = abs((int)(h / transformY));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= w) drawEndX = w - 1;

        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXTURE_SIZE / spriteWidth) / 256;

            if (transformY > 0 && stripe >= 0 && stripe < w && transformY < zBuffer[stripe]) {
                for (int y = drawStartY; y < drawEndY; y++) {
                    int d = (y) * 256 - h * 128 + spriteHeight * 128;
                    int texY = ((d * TEXTURE_SIZE) / spriteHeight) / 256;

                    Uint32 pixel = state->graphics.textureBuffers[sprites[spriteOrder[i]].texture_id][TEXTURE_SIZE * texY + texX];
                    
                    if ((pixel >> 24) > 0) {
                        Uint8 r = ((pixel >> 24) & 0xFF) / 2;
                        Uint8 g = ((pixel >> 16) & 0xFF) / 2;
                        Uint8 b = ((pixel >> 8 ) & 0xFF) / 2;
                        Uint8 a = pixel & 0xFF;

                        state->graphics.screenBuffers[y * w + stripe] = (a << 24) | (r << 16) | (g << 8) | b;
                    }
                }
            }
        }
    }

    free(spriteOrder);
    free(spriteDistance);
}
