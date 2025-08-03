/**
 * @file data.h
 * @brief Game save/load functionality for The Backrooms
 * 
 * This module provides functions for saving and loading game state data
 * to/from persistent storage. Handles player position, map state, and
 * discovered areas for save game functionality.
 */

#ifndef DATA_H
#define DATA_H

#include "../../main.h"
#include "../map/map.h"

// Error codes for data operations
#define DATA_SUCCESS            0
#define DATA_ERROR_FILE_ACCESS  1
#define DATA_ERROR_FORMAT       2
#define DATA_ERROR_CORRUPTION   3
#define DATA_ERROR_INVALID_PARAM 4

/**
 * @brief Saves the current game state to a file
 * 
 * Serializes the complete game state including player position, angle,
 * map dimensions, map data, and discovered areas to a save file.
 * 
 * @param state Pointer to the game state to save (must not be NULL)
 * @param nameSave Name of the save file (without extension)
 * 
 * @return DATA_SUCCESS on success, or error code:
 *         - DATA_ERROR_INVALID_PARAM: Invalid parameters
 *         - DATA_ERROR_FILE_ACCESS: Cannot create/write to file
 * 
 * @note Save files are stored in the "saves/" directory with .dat extension
 * @note Function creates the save directory if it doesn't exist
 */
extern int saveData(GameState *state, const char *nameSave);

/**
 * @brief Loads game state from a save file
 * 
 * Deserializes game state from a save file, restoring player position,
 * map data, and discovered areas. Validates file format and data integrity.
 * 
 * @param state Pointer to game state structure to populate (must not be NULL)
 * @param nameSave Name of the save file to load (without extension)
 * 
 * @return DATA_SUCCESS on success, or error code:
 *         - DATA_ERROR_INVALID_PARAM: Invalid parameters
 *         - DATA_ERROR_FILE_ACCESS: Cannot open/read file
 *         - DATA_ERROR_FORMAT: Invalid file format
 *         - DATA_ERROR_CORRUPTION: Data corruption detected
 * 
 * @note Function assumes map arrays are already allocated in GameState
 * @note Save files are expected in "saves/" directory with .dat extension
 */
extern int readData(GameState *state, const char *nameSave);

#endif /* DATA_H */