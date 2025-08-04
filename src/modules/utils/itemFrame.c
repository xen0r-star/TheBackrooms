#include "utils.h"


void itemFrame(const AppState appState, int selectFrame) {
    // Validate input parameters
    if (!appState.renderer) {
        fprintf(stderr, "Error: Invalid renderer for item frame\n");
        return;
    }
    
    // Validate frame selection range
    if (selectFrame < 1 || selectFrame > INVENTORY_MAX_SLOTS) {
        fprintf(stderr, "Warning: Invalid frame selection %d, clamping to valid range\n", selectFrame);
        selectFrame = (selectFrame < 1) ? 1 : INVENTORY_MAX_SLOTS;
    }

    SDL_Renderer *renderer = appState.renderer;
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    // Load inventory bar texture
    SDL_Surface *inventoryBarSurface = IMG_Load("textures/inventoryBar.png");
    if (!inventoryBarSurface) {
        fprintf(stderr, "Error loading inventory bar image: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *inventoryBar = SDL_CreateTextureFromSurface(renderer, inventoryBarSurface);
    SDL_FreeSurface(inventoryBarSurface);
    if (!inventoryBar) {
        fprintf(stderr, "Error creating inventory bar texture: %s\n", SDL_GetError());
        return;
    }

    // Load frame selection texture
    SDL_Surface *frameSelectSurface = IMG_Load("textures/frameSelect.png");
    if (!frameSelectSurface) {
        fprintf(stderr, "Error loading frame select image: %s\n", IMG_GetError());
        SDL_DestroyTexture(inventoryBar);
        return;
    }

    SDL_Texture *frameSelect = SDL_CreateTextureFromSurface(renderer, frameSelectSurface);
    SDL_FreeSurface(frameSelectSurface);
    if (!frameSelect) {
        fprintf(stderr, "Error creating frame select texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(inventoryBar);
        return;
    }

    // Calculate centered position for inventory bar
    int inventoryX = (screenWidth - INVENTORY_BAR_WIDTH) / 2;
    int inventoryY = screenHeight - INVENTORY_BAR_HEIGHT - INVENTORY_MARGIN_BOTTOM;
    
    // Render inventory bar
    SDL_Rect inventoryRect = {
        inventoryX,
        inventoryY,
        INVENTORY_BAR_WIDTH,
        INVENTORY_BAR_HEIGHT
    };
    SDL_RenderCopy(renderer, inventoryBar, NULL, &inventoryRect);

    // Calculate selection frame position
    // Frame positioning: base position + (frame - 1) * slot spacing
    int frameX = inventoryX + (selectFrame - 1) * INVENTORY_SLOT_SPACING;
    int frameY = inventoryY;
    
    // Render selection frame
    SDL_Rect frameRect = {
        frameX,
        frameY,
        INVENTORY_FRAME_SIZE,
        INVENTORY_FRAME_SIZE
    };
    SDL_RenderCopy(renderer, frameSelect, NULL, &frameRect);

    // Clean up textures
    SDL_DestroyTexture(inventoryBar);
    SDL_DestroyTexture(frameSelect);
}