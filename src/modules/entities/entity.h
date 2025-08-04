/**
 * @file entity.h
 * @brief Entity management and sprite rendering for The Backrooms
 * 
 * This module handles 3D sprite entities in the game world, including
 * rendering with proper depth sorting, transparency blending, and
 * perspective projection.
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "../../main.h"

// Error codes for entity operations
#define ENTITY_SUCCESS           0
#define ENTITY_ERROR_MEMORY     -1
#define ENTITY_ERROR_INVALID    -2
#define ENTITY_ERROR_OVERFLOW   -3

// Maximum number of sprites to prevent memory issues
#define MAX_SPRITES 1000

/**
 * @brief Renders all sprites in the game world with proper 3D projection
 * 
 * Performs 3D sprite rendering using perspective projection, depth sorting,
 * and alpha blending. Sprites are rendered back-to-front for correct
 * transparency handling.
 * 
 * @param state Pointer to the main game state
 * @param sprites Array of sprites to render
 * @param numSprites Number of sprites in the array
 * @param zBuffer Depth buffer for proper occlusion handling
 * 
 * @note Function requires valid zBuffer with screen width elements
 * @note Sprites are automatically sorted by distance from camera
 * @note Supports transparency and vertical movement effects
 */
extern void render_sprites(GameState *state, Sprite *sprites, int numSprites, float *zBuffer);

/**
 * @brief Adds a new sprite entity to the entity system
 * 
 * Dynamically expands the sprite array and adds a new sprite entity.
 * Handles memory allocation and error recovery.
 * 
 * @param state Pointer to the entity state to modify
 * @param sprite The sprite data to add to the system
 * 
 * @return ENTITY_SUCCESS on success, or error code:
 *         - ENTITY_ERROR_INVALID: Invalid parameters
 *         - ENTITY_ERROR_MEMORY: Memory allocation failed
 *         - ENTITY_ERROR_OVERFLOW: Too many sprites
 * 
 * @note Function uses realloc for dynamic memory management
 * @note Original sprite count is restored on allocation failure
 */
extern int initializationEntity(EntityState *state, Sprite sprite);

#endif /* ENTITY_H */
