/**
 * @file core.h
 * @brief Core raycasting functionality for The Backrooms game
 * 
 * This module provides essential raycasting algorithms used for 3D rendering
 * and collision detection in the game engine.
 */

#ifndef CORE_H
#define CORE_H

#include "../../main.h"


/**
 * @brief Casts a ray using DDA algorithm to find wall intersections
 * 
 * This function implements the Digital Differential Analyzer (DDA) algorithm
 * to trace a ray from the player's position in a specified direction until
 * it hits a wall. Used for 3D rendering and collision detection.
 * 
 * @param playerState Current player state containing position and orientation
 * @param mapState Pointer to map state (modified to mark discovered areas)
 * @param rayDirX X component of the ray direction vector (normalized)
 * @param rayDirY Y component of the ray direction vector (normalized)
 * @param distance Pointer to store the perpendicular distance to the wall
 * @param wallType Pointer to store the type/texture ID of the wall hit
 * @param wallSide Pointer to store which side was hit (0=X-side, 1=Y-side)
 * 
 * @return 0 on success, negative value on error:
 *         -1: Invalid input parameters (null pointers)
 *         -2: Ray exceeded maximum steps (infinite loop protection)
 * 
 * @note The function modifies mapState->mapDiscovered to implement fog-of-war
 * @note Distance calculation avoids fisheye effect for proper 3D rendering
 */
extern int castRay(const PlayerState playerState, MapState *mapState, 
                   float rayDirX, float rayDirY, 
                   float *distance, int *wallType, int *wallSide);

#endif /* CORE_H */