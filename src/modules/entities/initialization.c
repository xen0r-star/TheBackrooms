#include "entity.h"
#include <string.h>


static int validateEntityParams(const EntityState *state) {
    if (!state) {
        return ENTITY_ERROR_INVALID;
    }
    
    if (state->numSprites >= MAX_SPRITES) {
        return ENTITY_ERROR_OVERFLOW;
    }
    
    return ENTITY_SUCCESS;
}


int initializationEntity(EntityState *state, Sprite sprite) {
    // Validate input parameters
    int validation = validateEntityParams(state);
    if (validation != ENTITY_SUCCESS) {
        return validation;
    }

    // Store original count for error recovery
    int originalCount = state->numSprites;
    
    // Increment sprite count
    state->numSprites++;

    // Reallocate memory for expanded sprite array
    Sprite *newSprites = realloc(state->sprites, sizeof(Sprite) * state->numSprites);
    if (newSprites == NULL) {
        // Restore original count on allocation failure
        state->numSprites = originalCount;
        fprintf(stderr, "Error: Failed to allocate memory for %d sprites (%.2f KB)\n", 
                state->numSprites, (sizeof(Sprite) * state->numSprites) / 1024.0f);
        return ENTITY_ERROR_MEMORY;
    }

    // Update sprite array pointer
    state->sprites = newSprites;

    // Copy sprite data with bounds checking
    memcpy(&state->sprites[state->numSprites - 1], &sprite, sizeof(Sprite));

    // Validate the added sprite data
    Sprite *addedSprite = &state->sprites[state->numSprites - 1];
    if (!isfinite(addedSprite->x) || !isfinite(addedSprite->y) ||
        !isfinite(addedSprite->scaleX) || !isfinite(addedSprite->scaleY)) {
        // Invalid sprite data, remove it
        state->numSprites = originalCount;
        fprintf(stderr, "Warning: Invalid sprite data detected and rejected\n");
        return ENTITY_ERROR_INVALID;
    }

    return ENTITY_SUCCESS;
}