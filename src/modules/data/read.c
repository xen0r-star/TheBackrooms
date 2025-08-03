#include "data.h"
#include <ctype.h>


static int validateReadParams(const GameState *state, const char *nameSave) {
    if (!state || !nameSave) {
        return DATA_ERROR_INVALID_PARAM;
    }
    
    if (strlen(nameSave) == 0 || strlen(nameSave) >= 200) {
        return DATA_ERROR_INVALID_PARAM;
    }
    
    return DATA_SUCCESS;
}

static int skipComments(FILE *file) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '#') {
            // Skip entire comment line
            while ((c = fgetc(file)) != EOF && c != '\n');
        } else if (c == '\n' || isspace(c)) {
            // Skip whitespace
            continue;
        } else {
            // Found non-comment, non-whitespace character
            ungetc(c, file);
            break;
        }
    }
    return c;
}

static int validateMapDimensions(int width, int height) {
    const int MAX_MAP_SIZE = 1000; // Reasonable maximum
    const int MIN_MAP_SIZE = 1;
    
    if (width < MIN_MAP_SIZE || width > MAX_MAP_SIZE ||
        height < MIN_MAP_SIZE || height > MAX_MAP_SIZE) {
        return DATA_ERROR_CORRUPTION;
    }
    
    return DATA_SUCCESS;
}


int readData(GameState *state, const char *nameSave) {
    // Validate input parameters
    int validation = validateReadParams(state, nameSave);
    if (validation != DATA_SUCCESS) {
        return validation;
    }

    // Construct filename with bounds checking
    char fileName[256];
    int result = snprintf(fileName, sizeof(fileName), "saves/%s.dat", nameSave);
    if (result >= (int)sizeof(fileName) || result < 0) {
        return DATA_ERROR_INVALID_PARAM;
    }

    // Open file for reading
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return DATA_ERROR_FILE_ACCESS;
    }

    // Skip any header comments
    skipComments(file);

    // Read player data with improved error checking
    float playerX, playerY, playerAngle;
    if (fscanf(file, "PLAYER %f %f %f", &playerX, &playerY, &playerAngle) != 3) {
        fclose(file);
        return DATA_ERROR_FORMAT;
    }

    // Validate player position (basic sanity check)
    if (!isfinite(playerX) || !isfinite(playerY) || !isfinite(playerAngle)) {
        fclose(file);
        return DATA_ERROR_CORRUPTION;
    }

    // Skip comments and read map dimensions
    skipComments(file);
    
    int mapWidth, mapHeight;
    if (fscanf(file, "MAP_WIDTH %d", &mapWidth) != 1) {
        fclose(file);
        return DATA_ERROR_FORMAT;
    }

    skipComments(file);
    
    if (fscanf(file, "MAP_HEIGHT %d", &mapHeight) != 1) {
        fclose(file);
        return DATA_ERROR_FORMAT;
    }

    // Validate map dimensions
    if (validateMapDimensions(mapWidth, mapHeight) != DATA_SUCCESS) {
        fclose(file);
        return DATA_ERROR_CORRUPTION;
    }

    // Check if dimensions match allocated arrays (if applicable)
    if (state->mapState.map != NULL && 
        (mapWidth != state->mapState.mapWidth || mapHeight != state->mapState.mapHeight)) {
        fclose(file);
        return DATA_ERROR_FORMAT; // Dimension mismatch
    }

    // Skip comments before map data
    skipComments(file);

    // Read map data with bounds checking
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int mapValue;
            if (fscanf(file, "%d", &mapValue) != 1) {
                fclose(file);
                return DATA_ERROR_FORMAT;
            }
            
            // Validate map value (reasonable range check)
            if (mapValue < 0 || mapValue > 255) {
                fclose(file);
                return DATA_ERROR_CORRUPTION;
            }
            
            state->mapState.map[y][x] = mapValue;
        }
    }

    // Skip comments before discovered areas data
    skipComments(file);

    // Read discovered areas data
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int discoveredValue;
            if (fscanf(file, "%d", &discoveredValue) != 1) {
                fclose(file);
                return DATA_ERROR_FORMAT;
            }
            
            // Validate discovered value (should be 0 or 1)
            if (discoveredValue != 0 && discoveredValue != 1) {
                fclose(file);
                return DATA_ERROR_CORRUPTION;
            }
            
            state->mapState.mapDiscovered[y][x] = discoveredValue;
        }
    }

    // All data read successfully, now update the state
    state->playerState.player.x = playerX;
    state->playerState.player.y = playerY;
    state->playerState.player.angle = playerAngle;
    state->mapState.mapWidth = mapWidth;
    state->mapState.mapHeight = mapHeight;

    fclose(file);
    return DATA_SUCCESS;
}