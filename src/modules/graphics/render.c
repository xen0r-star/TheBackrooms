#include "graphics.h"

// Mathematical constants
#define INV_TEXTURE_SIZE (1.0f / TEXTURE_SIZE)

// Rendering constants
#define FOG_MAX_DISTANCE 30.0f      // Maximum fog distance
#define FOG_MAX_INTENSITY 0.5f      // Maximum fog opacity
#define SHADOW_FACTOR 2             // Side wall darkening factor
#define FOG_COLOR_R 170             // Fog color red component (textured mode)
#define FOG_COLOR_G 170             // Fog color green component (textured mode)
#define FOG_COLOR_B 170             // Fog color blue component (textured mode)

// Non-textured mode colors (ARGB format)
#define CEILING_COLOR 0xFF474112    // Dark yellowish ceiling
#define FLOOR_COLOR   0xFF524B1C    // Brown floor

// Wall type colors for non-textured rendering
static const Color WALL_COLORS[] = {
    {184, 181, 55, 255},  // Wall type 1: Yellow-beige
    {184, 181, 55, 255},  // Wall type 2: Yellow-beige (same as type 1)
    {0, 255, 0, 255},     // Wall type 3: Green
    {0, 0, 255, 255},     // Wall type 4: Blue
    {255, 0, 255, 255},   // Wall type 5: Magenta
};
#define NUM_WALL_COLORS (sizeof(WALL_COLORS) / sizeof(WALL_COLORS[0]))


static Color applyFogEffect(Color color, float distance, int fogR, int fogG, int fogB) {
    float fogFactor = distance / FOG_MAX_DISTANCE;
    if (fogFactor > FOG_MAX_INTENSITY) {
        fogFactor = FOG_MAX_INTENSITY;
    }
    fogFactor *= fogFactor; // Quadratic falloff for realistic fog
    
    color.r = (int)((1.0f - fogFactor) * color.r + fogFactor * fogR);
    color.g = (int)((1.0f - fogFactor) * color.g + fogFactor * fogG);
    color.b = (int)((1.0f - fogFactor) * color.b + fogFactor * fogB);
    
    return color;
}

static Color applySideWallShading(Color color, int wallSide) {
    if (wallSide == 1) {
        color.r /= SHADOW_FACTOR;
        color.g /= SHADOW_FACTOR;
        color.b /= SHADOW_FACTOR;
    }
    return color;
}

static Color getWallColor(int wallType) {
    if (wallType >= 1 && wallType <= (int)NUM_WALL_COLORS) {
        return WALL_COLORS[wallType - 1];
    }
    return (Color){0, 0, 0, 255}; // Default to black for unknown types
}

static void renderFloorAndCeiling(GameState *state, int screenWidth, int screenHeight,
                                 float dirX, float dirY, float planeX, float planeY) {
    // Calculate ray directions for leftmost and rightmost screen columns
    float rayDirX0 = dirX - planeX;
    float rayDirY0 = dirY - planeY;
    float rayDirX1 = dirX + planeX;
    float rayDirY1 = dirY + planeY;

    float posZ = 0.5f * screenHeight; // Height for horizontal plane casting

    // Render floor and ceiling for lower half of screen
    for (int y = screenHeight / 2 + 1; y < screenHeight; y++) {
        int p = y - screenHeight / 2; // Distance from screen center
        
        float rowDistance = posZ / (float)p; // Distance to current row
        
        // Calculate texture coordinate steps for current row
        float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
        float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

        // Starting world coordinates for current row
        float floorX = state->playerState.player.x + rowDistance * rayDirX0;
        float floorY = state->playerState.player.y + rowDistance * rayDirY0;

        for (int x = 0; x < screenWidth; ++x) {
            // Calculate texture coordinates
            int cellX = (int)(floorX);
            int cellY = (int)(floorY);

            int texX = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
            int texY = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);
            int texIndex = TEXTURE_SIZE * texY + texX;

            floorX += floorStepX;
            floorY += floorStepY;

            // Render floor pixel with darkening for depth
            Uint32 pixel = state->graphics.textureBuffers[3][texIndex]; // Floor texture
            Uint8 r = ((pixel >> 24) & 0xFF) / 2; // Darken for atmosphere
            Uint8 g = ((pixel >> 16) & 0xFF) / 2;
            Uint8 b = ((pixel >> 8) & 0xFF) / 2;
            Uint8 a = pixel & 0xFF;

            state->graphics.screenBuffers[y * screenWidth + x] = 
                (a << 24) | (r << 16) | (g << 8) | b;

            // Render ceiling pixel (mirrored position)
            pixel = state->graphics.textureBuffers[2][texIndex]; // Ceiling texture
            r = ((pixel >> 24) & 0xFF) / 2; // Darken for atmosphere
            g = ((pixel >> 16) & 0xFF) / 2;
            b = ((pixel >> 8) & 0xFF) / 2;
            a = pixel & 0xFF;

            state->graphics.screenBuffers[(screenHeight - y - 1) * screenWidth + x] = 
                (a << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

static void renderTexturedWalls(GameState *state, int screenWidth, int screenHeight,
                               float dirX, float dirY, float planeX, float planeY) {
    float invScreenWidth = 2.0f / screenWidth; // Optimization: pre-calculate

    for (int x = 0; x < screenWidth; x++) {
        // Calculate ray direction for current screen column
        float cameraX = x * invScreenWidth - 1.0f;
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        // Cast ray and get wall information
        float distance;
        int wallType, wallSide;
        castRay(state->playerState, &state->mapState, rayDirX, rayDirY, 
                &distance, &wallType, &wallSide);

        // Store distance for sprite depth sorting
        state->entityState.zBuffer[x] = distance;

        // Skip rendering if no wall hit (type 9 = no wall)
        if (wallType == 9) continue;

        // Calculate wall rendering bounds
        int wallHeight = (int)(screenHeight / distance);
        int drawStart = -wallHeight / 2 + screenHeight / 2;
        int drawEnd = wallHeight / 2 + screenHeight / 2;

        // Clamp to screen bounds
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // Calculate texture X coordinate
        double wallX;
        if (wallSide == 0) {
            wallX = state->playerState.player.y + distance * rayDirY;
        } else {
            wallX = state->playerState.player.x + distance * rayDirX;
        }
        wallX -= floor(wallX); // Get fractional part

        int texX = (int)(wallX * (double)TEXTURE_SIZE);
        
        // Flip texture coordinate for proper orientation
        if (wallSide == 0 && rayDirX > 0) texX = TEXTURE_SIZE - texX - 1;
        if (wallSide == 1 && rayDirY < 0) texX = TEXTURE_SIZE - texX - 1;

        // Calculate texture stepping
        double step = 1.0 * TEXTURE_SIZE / wallHeight;
        double texPos = (drawStart - screenHeight / 2 + wallHeight / 2) * step;

        // Render wall column
        for (int y = drawStart; y < drawEnd; y++) {
            int texY = (int)texPos & (TEXTURE_SIZE - 1);
            texPos += step;

            // Get texture pixel
            Uint32 pixel = state->graphics.textureBuffers[wallType - 1][TEXTURE_SIZE * texY + texX];
            Uint8 r = ((pixel >> 24) & 0xFF) / (wallSide == 1 ? SHADOW_FACTOR : 1);
            Uint8 g = ((pixel >> 16) & 0xFF) / (wallSide == 1 ? SHADOW_FACTOR : 1);
            Uint8 b = ((pixel >> 8) & 0xFF) / (wallSide == 1 ? SHADOW_FACTOR : 1);
            Uint8 a = pixel & 0xFF;

            // Apply fog effect with black fog for textured mode
            float fogFactor = distance / FOG_MAX_DISTANCE;
            if (fogFactor > FOG_MAX_INTENSITY) fogFactor = FOG_MAX_INTENSITY;
            fogFactor *= fogFactor;

            r = (Uint8)((1.0f - fogFactor) * r + fogFactor * 0);
            g = (Uint8)((1.0f - fogFactor) * g + fogFactor * 0);
            b = (Uint8)((1.0f - fogFactor) * b + fogFactor * 0);

            state->graphics.screenBuffers[y * screenWidth + x] = 
                (a << 24) | (r << 16) | (g << 8) | b;
        }
    }
}

static void renderWireframeWalls(GameState *state, int screenWidth, int screenHeight,
                                float dirX, float dirY, float planeX, float planeY) {
    float invScreenWidth = 2.0f / screenWidth; // Optimization: pre-calculate

    for (int x = 0; x < screenWidth; x++) {
        // Calculate ray direction for current screen column
        float cameraX = x * invScreenWidth - 1.0f;
        float rayDirX = dirX + planeX * cameraX;
        float rayDirY = dirY + planeY * cameraX;

        // Cast ray and get wall information
        float distance;
        int wallType, wallSide;
        castRay(state->playerState, &state->mapState, rayDirX, rayDirY, 
                &distance, &wallType, &wallSide);

        // Store distance for sprite depth sorting
        state->entityState.zBuffer[x] = distance;

        // Calculate wall rendering bounds
        int wallHeight = (int)(screenHeight / distance);
        int drawStart = (screenHeight - wallHeight) / 2;
        int drawEnd = drawStart + wallHeight;

        // Clamp to screen bounds
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // Get base wall color and apply effects
        Color color = getWallColor(wallType);
        color = applySideWallShading(color, wallSide);
        color = applyFogEffect(color, distance, FOG_COLOR_R, FOG_COLOR_G, FOG_COLOR_B);

        // Render entire screen column
        for (int y = 0; y < screenHeight; y++) {
            if (y < drawStart) {
                // Ceiling
                state->graphics.screenBuffers[y * screenWidth + x] = CEILING_COLOR;
            } else if (y > drawEnd) {
                // Floor
                state->graphics.screenBuffers[y * screenWidth + x] = FLOOR_COLOR;
            } else {
                // Wall
                state->graphics.screenBuffers[y * screenWidth + x] = 
                    (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
            }
        }
    }
}


void renderScene(GameState *state) {
    // Validate input parameters
    if (!state || !state->graphics.screenBuffers || !state->graphics.screenBuffersTexture) {
        fprintf(stderr, "Error: Invalid state or uninitialized graphics buffers\n");
        return;
    }

    // Get screen dimensions
    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;

    // Get camera direction from cached render data
    float dirX = state->graphics.renderCache.dirX;
    float dirY = state->graphics.renderCache.dirY;

    // Calculate camera plane (perpendicular to direction) for FOV
    float planeX = -dirY * state->graphics.renderCache.fovRender;
    float planeY = dirX * state->graphics.renderCache.fovRender;

    // Choose rendering mode based on player preference
    if (state->playerState.showTextures) {
        // === TEXTURED RENDERING MODE ===
        
        // Render floor and ceiling with textures
        renderFloorAndCeiling(state, screenWidth, screenHeight, dirX, dirY, planeX, planeY);
        
        // Render walls with textures
        renderTexturedWalls(state, screenWidth, screenHeight, dirX, dirY, planeX, planeY);
    } else {
        // === WIREFRAME/DEBUG RENDERING MODE ===
        
        // Render walls with solid colors (includes floor/ceiling)
        renderWireframeWalls(state, screenWidth, screenHeight, dirX, dirY, planeX, planeY);
    }

    // Render all sprites with proper depth sorting
    render_sprites(state, state->entityState.sprites, state->entityState.numSprites, 
                  state->entityState.zBuffer);

    // Update GPU texture with rendered pixel buffer
    SDL_UpdateTexture(state->graphics.screenBuffersTexture, NULL, 
                     state->graphics.screenBuffers, screenWidth * sizeof(Uint32));
    
    // Copy texture to screen
    SDL_RenderCopy(state->app.renderer, state->graphics.screenBuffersTexture, NULL, NULL);
}