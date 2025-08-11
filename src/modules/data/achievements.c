#include "achievements.h"
#include <string.h>
#include <math.h>
#include "../core/core.h"

static const Achievement DEFAULT_ACHIEVEMENTS[] = {
    {"first_steps", "First Steps", "Move for the first time.", 0, 1, false},
    {"cartographer", "Cartographer", "Discover 50 tiles.", 0, 50, false},
    {"speed_runner", "Speed Runner", "Reach 10 FPS movement speed.", 0, 1, false},
    {"stalker_spotted", "I See You", "Catch the entity looking at you.", 0, 1, false},
    {"close_call", "Close Call", "Survive near the entity.", 0, 1, false},
    {"wanderer", "Wanderer", "Walk 200 tiles.", 0, 200, false},
    {"switch_flicker", "Switch Flicker", "Toggle collision 5 times.", 0, 5, false},
    {"collector", "Collector", "Open inventory 50 times.", 0, 50, false},
    {"nightmare", "Nightmare", "Trigger the glitch effect.", 0, 1, false},
    {"escaped", "Escaped?", "Find an exit.", 0, 1, false},
};

int achievements_init(GameState *state) {
    if (!state) return -1;
    // Free previous
    if (state->achievements.items) {
        free(state->achievements.items);
        state->achievements.items = NULL;
        state->achievements.count = 0;
    }
    int count = (int)(sizeof(DEFAULT_ACHIEVEMENTS)/sizeof(DEFAULT_ACHIEVEMENTS[0]));
    state->achievements.items = (Achievement*)malloc(sizeof(Achievement)*count);
    if (!state->achievements.items) return -1;
    memcpy(state->achievements.items, DEFAULT_ACHIEVEMENTS, sizeof(DEFAULT_ACHIEVEMENTS));
    state->achievements.count = count;
    return 0;
}

static Achievement* find(GameState *state, const char *id) {
    if (!state || !id || !state->achievements.items) return NULL;
    for (int i=0;i<state->achievements.count;i++) {
        if (strcmp(state->achievements.items[i].id, id)==0) return &state->achievements.items[i];
    }
    return NULL;
}

void achievement_set_progress(GameState *state, const char *id, int value) {
    Achievement *a = find(state, id);
    if (!a || a->unlocked) return;
    a->progress = value;
    if (a->target <= 0) {
        a->unlocked = a->progress > 0;
    } else if (a->progress >= a->target) {
        a->progress = a->target;
        a->unlocked = true;
        // TODO: playAchievementSound();
    }
}

void achievement_add_progress(GameState *state, const char *id, int delta) {
    Achievement *a = find(state, id);
    if (!a || a->unlocked) return;
    achievement_set_progress(state, id, a->progress + delta);
}

void achievement_unlock(GameState *state, const char *id) {
    Achievement *a = find(state, id);
    if (!a) return;
    a->unlocked = true;
    a->progress = (a->target>0)?a->target:1;
    // TODO: playAchievementSound();
}

int achievements_load(GameState *state, const char *saveId) {
    (void)state; (void)saveId; return 0; // stub
}
int achievements_save(GameState *state, const char *saveId) {
    (void)state; (void)saveId; return 0; // stub
}

void achievements_update(GameState *state) {
    if (!state || !state->achievements.items) return;

    // Track movement distance (approx): add playerMoveSpeed per frame
    if (state->playerState.playerMoveSpeed > 0.0f) {
        achievement_add_progress(state, "first_steps", 1);
        // Accumulate distance walked (approx in units)
        int add = (int)ceilf(state->playerState.playerMoveSpeed);
        if (add > 0) achievement_add_progress(state, "wanderer", add);
    }

    // Speed runner: high per-frame speed (frame-time independent threshold)
    if (state->playerState.playerMoveSpeed >= 4.5f) {
        achievement_unlock(state, "speed_runner");
    }

    // Discover tiles count: scan mapDiscovered if available
    if (state->mapState.mapDiscovered) {
        int discovered = 0;
        for (int y=0;y<state->mapState.mapHeight;y++) {
            for (int x=0;x<state->mapState.mapWidth;x++) {
                if (state->mapState.mapDiscovered[y][x]) discovered++;
            }
        }
        achievement_set_progress(state, "cartographer", discovered);
    }

    // Close Call: entity very near the player
    for (int i=0; i<state->entityState.numSprites; ++i) {
        float dx = (float)state->entityState.sprites[i].x - state->playerState.player.x;
        float dy = (float)state->entityState.sprites[i].y - state->playerState.player.y;
        float d2 = dx*dx + dy*dy;
        if (d2 < 1.5f*1.5f) {
            achievement_unlock(state, "close_call");
            break;
        }
    }

    // I See You: player is looking directly at an entity with line of sight
    float dirX = state->graphics.renderCache.dirX;
    float dirY = state->graphics.renderCache.dirY;
    float cosCone = cosf(20.0f * (float)PI / 180.0f);
    for (int i=0; i<state->entityState.numSprites; ++i) {
        float vx = (float)state->entityState.sprites[i].x - state->playerState.player.x;
        float vy = (float)state->entityState.sprites[i].y - state->playerState.player.y;
        float len = sqrtf(vx*vx + vy*vy);
        if (len < 0.001f) continue;
        float nx = vx/len, ny = vy/len;
        float dot = nx*dirX + ny*dirY;
        if (dot > cosCone) {
            // LOS check using castRay
            float distance; int wallType, wallSide;
            castRay(state->playerState, &state->mapState, nx, ny, &distance, &wallType, &wallSide);
            (void)wallSide; (void)wallType;
            if (distance > len - 0.1f) {
                achievement_unlock(state, "stalker_spotted");
                break;
            }
        }
    }
}
