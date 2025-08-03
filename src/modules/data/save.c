#include "data.h"
#include <sys/stat.h>
#include <errno.h>

// Platform-specific includes for directory creation
#ifdef _WIN32
    #include <direct.h>  // For _mkdir on Windows
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// Ensure the saves directory exists, create it if not
static int ensureSaveDirectory(void) {
#ifdef _WIN32
    struct _stat st = {0};
    if (_stat("saves", &st) == -1) {
        return _mkdir("saves");
    }
#else
    struct stat st = {0};
    if (stat("saves", &st) == -1) {
        return mkdir("saves", 0755);
    }
#endif
    return 0;
}


static int validateSaveParams(const GameState *state, const char *nameSave) {
    if (!state || !nameSave) {
        return DATA_ERROR_INVALID_PARAM;
    }
    
    if (strlen(nameSave) == 0 || strlen(nameSave) >= 200) {
        return DATA_ERROR_INVALID_PARAM;
    }
    
    // Check for invalid characters in filename
    const char *invalidChars = "<>:\"/\\|?*";
    if (strpbrk(nameSave, invalidChars) != NULL) {
        return DATA_ERROR_INVALID_PARAM;
    }
    
    return DATA_SUCCESS;
}


int saveData(GameState *state, const char *nameSave) {
    // Validate input parameters
    int validation = validateSaveParams(state, nameSave);
    if (validation != DATA_SUCCESS) {
        return validation;
    }

    // Ensure save directory exists
    if (ensureSaveDirectory() != 0) {
        return DATA_ERROR_FILE_ACCESS;
    }

    // Construct filename with bounds checking
    char fileName[256];
    int result = snprintf(fileName, sizeof(fileName), "saves/%s.dat", nameSave);
    if (result >= (int)sizeof(fileName) || result < 0) {
        return DATA_ERROR_INVALID_PARAM; // Filename too long
    }

    // Open file for writing
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return DATA_ERROR_FILE_ACCESS;
    }

    // Write file header and version for future compatibility
    fprintf(file, "# The Backrooms Save File v1.0\n");
    fprintf(file, "# Generated on: %s", ctime(&(time_t){time(NULL)}));
    fprintf(file, "\n");

    // Write player data with consistent format
    fprintf(file, "PLAYER %.6f %.6f %.6f\n", 
            state->playerState.player.x, 
            state->playerState.player.y, 
            state->playerState.player.angle);

    fprintf(file, "\n");

    // Write map dimensions
    fprintf(file, "MAP_WIDTH %d\n", state->mapState.mapWidth);
    fprintf(file, "MAP_HEIGHT %d\n", state->mapState.mapHeight);

    fprintf(file, "\n");

    // Write map data with proper formatting
    fprintf(file, "# Map Layout\n");
    for (int y = 0; y < state->mapState.mapHeight; y++) {
        for (int x = 0; x < state->mapState.mapWidth; x++) {
            fprintf(file, "%d", state->mapState.map[y][x]);
            if (x < state->mapState.mapWidth - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }

    fprintf(file, "\n");

    // Write discovered areas data
    fprintf(file, "# Discovered Areas\n");
    for (int y = 0; y < state->mapState.mapHeight; y++) {
        for (int x = 0; x < state->mapState.mapWidth; x++) {
            fprintf(file, "%d", state->mapState.mapDiscovered[y][x]);
            if (x < state->mapState.mapWidth - 1) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }

    // Close file and check for write errors
    if (fclose(file) != 0) {
        return DATA_ERROR_FILE_ACCESS;
    }

    return DATA_SUCCESS;
}