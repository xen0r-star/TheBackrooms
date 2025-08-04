/**
 * @file map.h
 * @brief Procedural map generation system for The Backrooms
 * 
 * This module provides procedural generation of maze-like maps using
 * predefined room patterns, rotation algorithms, and connectivity
 * systems to create the infinite, monotonous feel of the Backrooms.
 */

#ifndef MAP_H
#define MAP_H

#include "../../main.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Map generation constants
#define MAX_ROOM_PATTERNS     7
#define DEFAULT_TILE_SIZE    12
#define MAX_REGIONS         100
#define MIN_REGION_SIZE      10
#define MAX_GENERATION_TRIES 20

// Tile type constants
#define TILE_EMPTY           0   // Walkable floor
#define TILE_WALL            1   // Solid wall
#define TILE_DECORATIVE      2   // Decorative wall elements
#define TILE_SPAWN           9   // Player spawn point

/**
 * @brief Room pattern definition for procedural generation
 * 
 * Defines a template room that can be rotated and placed in the map.
 * Uses ASCII art representation for easy visualization and editing.
 */
typedef struct {
    int width;              ///< Width of the room pattern
    int height;             ///< Height of the room pattern
    const char** tiles;     ///< ASCII representation ('.' = empty, '#' = wall, 'D' = door)
    int maxOccurrences;     ///< Maximum times this pattern can be used (-1 = unlimited)
} RoomPattern;

/**
 * @brief Generates a procedural map using room patterns
 * 
 * Creates a maze-like map by placing predefined room patterns in a grid,
 * connecting isolated regions with corridors, and ensuring full connectivity.
 * 
 * @param mapState Map state to populate with generated data
 * @param numberRoomWidth Number of room slots horizontally
 * @param numberRoomHeight Number of room slots vertically
 * 
 * @note Function automatically handles memory management for existing maps
 * @note Generated maps are guaranteed to be fully connected
 */
extern void generateMap(MapState *mapState, int numberRoomWidth, int numberRoomHeight);

/**
 * @brief Spawns player at designated spawn point in map
 * 
 * Locates the spawn tile (type 9) in the map and positions the player
 * at that location. Falls back to default position if no spawn found.
 * 
 * @param playerState Player state to update with spawn position
 * @param mapState Map state containing spawn information
 * 
 * @note Spawn tile is converted to empty space after player placement
 * @note Function guarantees valid player position even without spawn tile
 */
extern void spawnPlayerFromMap(PlayerState *playerState, MapState *mapState);

#endif /* MAP_H */