#include "entity.h"
#include <math.h>
#include <stdlib.h>


typedef struct
{
    int index;
    double distance;
} SpriteDistanceData;

static int compareSpriteDistance(const void *a, const void *b)
{
    const SpriteDistanceData *dataA = (const SpriteDistanceData *)a;
    const SpriteDistanceData *dataB = (const SpriteDistanceData *)b;

    if (dataA->distance > dataB->distance)
        return -1; // Far to near
    if (dataA->distance < dataB->distance)
        return 1;
    return 0;
}


static inline Uint32 blendPixels(Uint32 spritePixel, Uint32 bgPixel, float transparency)
{
    Uint8 spriteR = ((spritePixel >> 24) & 0xFF) / 2;
    Uint8 spriteG = ((spritePixel >> 16) & 0xFF) / 2;
    Uint8 spriteB = ((spritePixel >> 8) & 0xFF) / 2;

    if (transparency >= 1.0f || transparency <= 0.0f)
    {
        return (0xFF << 24) | (spriteR << 16) | (spriteG << 8) | spriteB;
    }

    // Lire la couleur du fond
    Uint8 bgR = (bgPixel >> 16) & 0xFF;
    Uint8 bgG = (bgPixel >> 8) & 0xFF;
    Uint8 bgB = bgPixel & 0xFF;

    // Mélange RGB avec fond
    Uint8 finalR = (Uint8)(spriteR * transparency + bgR * (1.0f - transparency));
    Uint8 finalG = (Uint8)(spriteG * transparency + bgG * (1.0f - transparency));
    Uint8 finalB = (Uint8)(spriteB * transparency + bgB * (1.0f - transparency));

    return (0xFF << 24) | (finalR << 16) | (finalG << 8) | finalB;
}

void render_sprites(GameState *state, Sprite *sprites, int numSprites, float *zBuffer)
{
    // Validate input parameters
    if (!state || !sprites || !zBuffer || numSprites <= 0)
        return;

    // Get player position and camera parameters
    double posX = state->playerState.player.x;
    double posY = state->playerState.player.y;

    float dirX = state->graphics.renderCache.dirX;
    float dirY = state->graphics.renderCache.dirY;

    // Calculate camera plane perpendicular to direction for FOV
    float planeX = -dirY * state->graphics.renderCache.fovRender;
    float planeY = dirX * state->graphics.renderCache.fovRender;

    const int w = state->app.screenWidth;
    const int h = state->app.screenHeight;

    // Allocate arrays for sprite sorting
    SpriteDistanceData *spriteData = malloc(numSprites * sizeof(SpriteDistanceData));
    if (!spriteData)
    {
        fprintf(stderr, "Error: Failed to allocate memory for sprite sorting\n");
        return;
    }

    // Calculate distances from camera to sprites
    for (int i = 0; i < numSprites; i++)
    {
        spriteData[i].index = i;
        // Use squared distance for performance (avoid sqrt)
        double dx = posX - sprites[i].x;
        double dy = posY - sprites[i].y;
        spriteData[i].distance = dx * dx + dy * dy;
    }

    // Sort sprites by distance (far to near) for proper alpha blending
    qsort(spriteData, numSprites, sizeof(SpriteDistanceData), compareSpriteDistance);

    // Calculate transformation matrix inverse determinant
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    if (!isfinite(invDet))
    {
        free(spriteData);
        return; // Degenerate camera matrix
    }

    // Render each sprite in sorted order
    for (int i = 0; i < numSprites; i++)
    {
        int spriteIndex = spriteData[i].index;

        // Get sprite properties with safety checks
        float vDiv = sprites[spriteIndex].scaleY;
        float uDiv = sprites[spriteIndex].scaleX;
        float vMove = sprites[spriteIndex].transform.moveY;
        float transparency = sprites[spriteIndex].transform.transparency;

        // Prevent division by zero
        if (vDiv <= 0.0f)
            vDiv = 1.0f;
        if (uDiv <= 0.0f)
            uDiv = 1.0f;

        // Clamp transparency to valid range
        if (transparency < 0.0f)
            transparency = 0.0f;
        if (transparency > 1.0f)
            transparency = 1.0f;

        // Calculate sprite position relative to camera
        double spriteX = sprites[spriteIndex].x - posX;
        double spriteY = sprites[spriteIndex].y - posY;

        // Transform sprite to camera space
        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        // Skip sprites behind the camera
        if (transformY <= 0.0)
            continue;

        // Calculate screen position
        int spriteScreenX = (int)((w / 2) * (1 + transformX / transformY));
        int vMoveScreen = (int)(-vMove / transformY);

        // Calculate sprite dimensions on screen
        int spriteHeight = abs((int)(h / transformY)) / (1.0f / vDiv);

        int drawStartY = -spriteHeight / 2 + h / 2 + vMoveScreen;
        // Clamp vertical drawing bounds
        if (drawStartY < 0)
            drawStartY = 0;
        int drawEndY = spriteHeight / 2 + h / 2 + vMoveScreen;
        if (drawEndY >= h)
            drawEndY = h - 1;

        // Calculate sprite width on screen
        int spriteWidth = abs((int)(h / transformY)) / (1.0f / uDiv);

        // Calculate horizontal drawing bounds
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= w)
            drawEndX = w - 1;

        // Skip if sprite is completely off-screen
        if (drawStartX >= drawEndX || drawStartY >= drawEndY)
            continue;

        // Render sprite column by column
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            // Calculate texture X coordinate
            int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXTURE_SIZE / spriteWidth) / 256;

            // Clamp texture coordinate
            if (texX < 0)
                texX = 0;
            if (texX >= TEXTURE_SIZE)
                texX = TEXTURE_SIZE - 1;

            // Depth test and bounds check
            if (transformY > 0 && stripe >= 0 && stripe < w && transformY < zBuffer[stripe])
            {
                // Render sprite column pixel by pixel
                for (int y = drawStartY; y < drawEndY; y++)
                {
                    // Calculate texture Y coordinate
                    int d = (y - vMoveScreen) * 256 - h * 128 + spriteHeight * 128;
                    int texY = ((d * TEXTURE_SIZE) / spriteHeight) / 256;

                    // Clamp texture coordinate
                    if (texY < 0)
                        texY = 0;
                    if (texY >= TEXTURE_SIZE)
                        texY = TEXTURE_SIZE - 1;

                    // Get pixel from texture
                    Uint32 pixel = state->graphics.textureBuffers[sprites[spriteIndex].texture_id][TEXTURE_SIZE * texY + texX];

                    // Check alpha channel (transparency test)
                    Uint8 originalAlpha = pixel & 0xFF;
                    if (originalAlpha > 0)
                    {
                        // Get background pixel for blending
                        Uint32 bgPixel = state->graphics.screenBuffers[y * w + stripe];

                        // Blend sprite pixel with background
                        Uint32 finalPixel = blendPixels(pixel, bgPixel, transparency);

                        // Write final pixel to screen buffer
                        state->graphics.screenBuffers[y * w + stripe] = finalPixel;
                    }
                }
            }
        }

        // Clean up allocated memory
        free(spriteData);
    }
}