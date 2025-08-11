/**
 * @file achievements.h
 * @brief Achievements system API
 */

#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "../../main.h"

// Initialize the achievements list with default entries
int achievements_init(GameState *state);

// Persist/load achievements (optional; integrate with save system later)
int achievements_load(GameState *state, const char *saveId);
int achievements_save(GameState *state, const char *saveId);

// Update helper: set progress or unlock
void achievement_set_progress(GameState *state, const char *id, int value);
void achievement_add_progress(GameState *state, const char *id, int delta);
void achievement_unlock(GameState *state, const char *id);

// Frame update: compute simple achievements like movement and discovery
void achievements_update(GameState *state);

#endif
