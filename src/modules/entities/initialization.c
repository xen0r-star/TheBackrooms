#include "entity.h"

int initializationEntity(EntityState *state, Sprite sprite) {
    state->numSprites++;

    state->sprites = realloc(state->sprites, sizeof(Sprite) * state->numSprites);
    if (state->sprites == NULL) {
        fprintf(stderr, "Failed to allocate memory for sprites.\n");
        state->numSprites--;
        return 1;
    }

    state->sprites[state->numSprites - 1] = sprite;

    return 0;
}