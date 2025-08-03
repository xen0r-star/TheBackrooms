#include "map.h"

/**
 * 
 * Array of room templates used to create diverse map layouts.
 * Each pattern defines dimensions, tile layout, and usage limits.
 * 
 * Pattern symbols:
 * - '.' = Empty space (walkable)
 * - '#' = Wall (solid)
 * - 'D' = Door/special tile
 */
RoomPattern allRooms[MAX_ROOM_PATTERNS] = {
    {
        .width = 10,
        .height = 10,
        .tiles = (const char*[]) {
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            "..........",
            ".........."
        },
        .maxOccurrences = 3
    },
    {
        .width = 10,
        .height = 10,
        .tiles = (const char*[]) {
            "..........",
            "..........",
            "...####...",
            "...####...",
            "...####...",
            "...####...",
            "...####...",
            "...####...",
            "..........",
            ".........."
        },
        .maxOccurrences = -1
    },
    {
        .width = 40,
        .height = 40,
        .tiles = (const char*[]) {
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            ".#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#",
            "........................................",
            "........................................",
        },
        .maxOccurrences = 2
    },
    {
        .width = 10,
        .height = 10,
        .tiles = (const char*[]) {
            "..........",
            "..........",
            "...#####..",
            "...#......",
            "...#####..",
            "...#......",
            "...#......",
            "..........",
            "..........",
            ".........."
        },
        .maxOccurrences = 2
    },
    {
        .width = 10,
        .height = 10,
        .tiles = (const char*[]) {
            "..........",
            "..........",
            "..######..",
            "..#....#..",
            "....##....",
            "..#....#..",
            "..######..",
            "..........",
            "..........",
            ".........."
        },
        .maxOccurrences = -1
    },
    {
        .width = 10,
        .height = 10,
        .tiles = (const char*[]) {
            "..........",
            "##########",
            "..........",
            "##########",
            "..........",
            "##########",
            "..........",
            "##########",
            "..........",
            "##########",
        },
        .maxOccurrences = -1
    },
    {
        .width = 10,
        .height = 10,
        .tiles = (const char*[]) {
            "##########",
            "#........#",
            "#........#",
            "######D###",
            "..........",
            "######D###",
            "#........#",
            "#........#",
            "##########",
            "##########"
        },
        .maxOccurrences = -1
    }
};

RoomPattern rotateRoom(RoomPattern *room, int rotation) {
    RoomPattern result;
    
    // Validate input
    if (!room || !room->tiles) {
        fprintf(stderr, "Error: Invalid room pattern for rotation\n");
        result.width = 0;
        result.height = 0;
        result.tiles = NULL;
        result.maxOccurrences = 0;
        return result;
    }
    
    // No rotation needed
    if (rotation == 0) {
        return *room;
    }

    int w = room->width;
    int h = room->height;

    // Determine new dimensions based on rotation
    if (rotation == 90 || rotation == 270) {
        result.width = h;
        result.height = w;
    } else {
        result.width = w;
        result.height = h;
    }
    
    result.maxOccurrences = room->maxOccurrences;

    // Allocate memory for rotated pattern
    const char **tiles = malloc(result.height * sizeof(char*));
    if (!tiles) {
        fprintf(stderr, "Error: Memory allocation failed for room rotation\n");
        result.width = 0;
        result.height = 0;
        result.tiles = NULL;
        return result;
    }
    
    for (int y = 0; y < result.height; y++) {
        tiles[y] = malloc((result.width + 1) * sizeof(char));
        if (!tiles[y]) {
            // Cleanup on allocation failure
            for (int cleanup = 0; cleanup < y; cleanup++) {
                free((void*)tiles[cleanup]);
            }
            free(tiles);
            fprintf(stderr, "Error: Memory allocation failed for room row\n");
            result.width = 0;
            result.height = 0;
            result.tiles = NULL;
            return result;
        }
    }

    // Perform rotation transformation
    for (int y = 0; y < result.height; y++) {
        for (int x = 0; x < result.width; x++) {
            char c;
            switch (rotation) {
                case 90:   // 90 degrees clockwise
                    c = room->tiles[w - 1 - x][y];
                    break;
                case 180:  // 180 degrees
                    c = room->tiles[h - 1 - y][w - 1 - x];
                    break;
                case 270:  // 270 degrees clockwise (90 counter-clockwise)
                    c = room->tiles[x][h - 1 - y];
                    break;
                default:   // Invalid rotation, no change
                    c = room->tiles[y][x];
                    break;
            }
            ((char *)tiles[y])[x] = c;
        }
        ((char *)tiles[y])[result.width] = '\0';  // Null-terminate string
    }

    result.tiles = tiles;
    return result;
}

void floodFill(int **map, int width, int height, int **regionMap, int x, int y, int regionId) {
    // Boundary checks
    if (x < 0 || y < 0 || x >= width || y >= height)
        return;
        
    // Skip if not empty space or already processed
    if (map[y][x] != TILE_EMPTY || regionMap[y][x] != -1)
        return;

    // Mark this tile as belonging to the current region
    regionMap[y][x] = regionId;

    // Recursively fill adjacent tiles (4-way connectivity)
    floodFill(map, width, height, regionMap, x + 1, y, regionId);
    floodFill(map, width, height, regionMap, x - 1, y, regionId);
    floodFill(map, width, height, regionMap, x, y + 1, regionId);
    floodFill(map, width, height, regionMap, x, y - 1, regionId);
}

void connectIsolatedZones(MapState *mapState) {
    if (!mapState || !mapState->map) {
        fprintf(stderr, "Error: Invalid map state for zone connection\n");
        return;
    }
    
    int width = mapState->mapWidth;
    int height = mapState->mapHeight;

    // Allocate region mapping array
    int **regionMap = malloc(height * sizeof(int *));
    if (!regionMap) {
        fprintf(stderr, "Error: Memory allocation failed for region mapping\n");
        return;
    }
    
    for (int y = 0; y < height; y++) {
        regionMap[y] = malloc(width * sizeof(int));
        if (!regionMap[y]) {
            // Cleanup on allocation failure
            for (int cleanup = 0; cleanup < y; cleanup++) {
                free(regionMap[cleanup]);
            }
            free(regionMap);
            fprintf(stderr, "Error: Memory allocation failed for region row\n");
            return;
        }
        
        // Initialize all tiles as unprocessed
        for (int x = 0; x < width; x++) {
            regionMap[y][x] = -1;
        }
    }

    // Identify separate regions using flood fill
    int regionCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mapState->map[y][x] == TILE_EMPTY && regionMap[y][x] == -1) {
                if (regionCount < MAX_REGIONS) {
                    floodFill(mapState->map, width, height, regionMap, x, y, regionCount);
                    regionCount++;
                }
            }
        }
    }

    // Connect all regions (1 and above) to region 0 (main region)
    for (int r = 1; r < regionCount; r++) {
        // Calculate region size to determine if connection is worthwhile
        int regionSize = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (regionMap[y][x] == r)
                    regionSize++;
            }
        }

        // Skip regions that are too small to be worth connecting
        if (regionSize < MIN_REGION_SIZE)
            continue;

        // Find closest points between current region and main region (0)
        int minDist = 99999;
        int ax = -1, ay = -1, bx = -1, by = -1;

        for (int y1 = 0; y1 < height; y1++) {
            for (int x1 = 0; x1 < width; x1++) {
                if (regionMap[y1][x1] != r) continue;

                for (int y2 = 0; y2 < height; y2++) {
                    for (int x2 = 0; x2 < width; x2++) {
                        if (regionMap[y2][x2] != 0) continue;

                        // Calculate Manhattan distance
                        int dist = abs(x1 - x2) + abs(y1 - y2);
                        if (dist < minDist) {
                            minDist = dist;
                            ax = x1; ay = y1;  // Point in current region
                            bx = x2; by = y2;  // Point in main region
                        }
                    }
                }
            }
        }

        // Create L-shaped corridor with random pivot point for variety
        if (ax != -1 && ay != -1 && bx != -1 && by != -1) {
            int pivotX = ax + (rand() % (abs(bx - ax) + 1)) * ((bx > ax) ? 1 : -1);
            int pivotY = by;

            // Create horizontal segment to pivot point
            int cx = ax;
            while (cx != pivotX) {
                if (ay >= 0 && ay < height && cx >= 0 && cx < width) {
                    mapState->map[ay][cx] = TILE_EMPTY;
                }
                cx += (pivotX > cx) ? 1 : -1;
            }

            // Create vertical segment to pivot Y
            int cy = ay;
            while (cy != pivotY) {
                if (cy >= 0 && cy < height && pivotX >= 0 && pivotX < width) {
                    mapState->map[cy][pivotX] = TILE_EMPTY;
                }
                cy += (pivotY > cy) ? 1 : -1;
            }

            // Create final horizontal segment to destination
            cx = pivotX;
            while (cx != bx) {
                if (by >= 0 && by < height && cx >= 0 && cx < width) {
                    mapState->map[by][cx] = TILE_EMPTY;
                }
                cx += (bx > cx) ? 1 : -1;
            }
        }
    }

    // Cleanup region mapping memory
    for (int y = 0; y < height; y++) {
        free(regionMap[y]);
    }
    free(regionMap);
}


void generateMap(MapState *mapState, int numberRoomWidth, int numberRoomHeight) {
    // Validate input parameters
    if (!mapState) {
        fprintf(stderr, "Error: Invalid map state for generation\n");
        return;
    }
    
    if (numberRoomWidth <= 0 || numberRoomHeight <= 0) {
        fprintf(stderr, "Error: Invalid room dimensions (%d x %d)\n", 
                numberRoomWidth, numberRoomHeight);
        return;
    }

    // Clean up existing map data
    if (mapState->map != NULL) {
        for (int i = 0; i < mapState->mapHeight; i++) {
            free(mapState->map[i]);
        }
        free(mapState->map);
        mapState->map = NULL;
    }

    if (mapState->mapDiscovered != NULL) {
        for (int i = 0; i < mapState->mapHeight; i++) {
            free(mapState->mapDiscovered[i]);
        }
        free(mapState->mapDiscovered);
        mapState->mapDiscovered = NULL;
    }

    // Calculate map dimensions
    int tileSize = DEFAULT_TILE_SIZE;
    int totalWidth = numberRoomWidth * tileSize + 1;
    int totalHeight = numberRoomHeight * tileSize + 1;
    mapState->mapWidth = totalWidth;
    mapState->mapHeight = totalHeight;

    // Allocate main map array
    mapState->map = malloc(totalHeight * sizeof(int *));
    if (!mapState->map) {
        fprintf(stderr, "Error: Memory allocation failed for main map\n");
        return;
    }
    
    // Allocate discovery tracking array
    mapState->mapDiscovered = malloc(totalHeight * sizeof(int *));
    if (!mapState->mapDiscovered) {
        fprintf(stderr, "Error: Memory allocation failed for discovery map\n");
        free(mapState->map);
        mapState->map = NULL;
        return;
    }

    // Allocate and initialize map rows
    for (int y = 0; y < totalHeight; y++) {
        mapState->map[y] = malloc(totalWidth * sizeof(int));
        mapState->mapDiscovered[y] = malloc(totalWidth * sizeof(int));
        
        if (!mapState->map[y] || !mapState->mapDiscovered[y]) {
            fprintf(stderr, "Error: Memory allocation failed for map row %d\n", y);
            // Cleanup on failure
            for (int cleanup = 0; cleanup <= y; cleanup++) {
                if (cleanup < y || !mapState->map[y]) {
                    free(mapState->map[cleanup]);
                    free(mapState->mapDiscovered[cleanup]);
                }
            }
            free(mapState->map);
            free(mapState->mapDiscovered);
            mapState->map = NULL;
            mapState->mapDiscovered = NULL;
            return;
        }
        
        // Initialize all tiles as walls and undiscovered
        for (int x = 0; x < totalWidth; x++) {
            mapState->map[y][x] = TILE_WALL;
            mapState->mapDiscovered[y][x] = 0;
        }
    }

    // Initialize random seed for consistent generation variation
    srand((unsigned int)time(NULL));
    
    // Track room pattern usage to enforce limits
    int roomUsageCount[MAX_ROOM_PATTERNS] = {0};

    // Track previously placed rooms to avoid repetition
    int previousRooms[numberRoomHeight][numberRoomWidth];
    for (int y = 0; y < numberRoomHeight; y++) {
        for (int x = 0; x < numberRoomWidth; x++) {
            previousRooms[y][x] = -1;  // No room placed
        }
    }

    // Place rooms in grid pattern
    for (int roomY = 0; roomY < numberRoomHeight; roomY++) {
        for (int roomX = 0; roomX < numberRoomWidth; roomX++) {
            // 10% chance to leave slot empty for variety
            if (rand() % 10 == 0) continue;

            int roomIndex;
            int tries = 0;
            
            // Select appropriate room pattern with constraints
            do {
                roomIndex = rand() % MAX_ROOM_PATTERNS;

                // Check occurrence limit for this pattern
                RoomPattern *tryRoom = &allRooms[roomIndex];
                if (tryRoom->maxOccurrences != -1 &&
                    roomUsageCount[roomIndex] >= tryRoom->maxOccurrences) {
                    tries++;
                    continue;
                }

                // Avoid repeating identical rooms adjacent to each other
                if ((roomX > 0 && previousRooms[roomY][roomX - 1] == roomIndex) ||
                    (roomY > 0 && previousRooms[roomY - 1][roomX] == roomIndex)) {
                    tries++;
                    continue;
                }

                break;
            } while (tries < MAX_GENERATION_TRIES);

            // Skip this slot if no suitable room found
            if (tries >= MAX_GENERATION_TRIES) continue;

            // Record room placement
            previousRooms[roomY][roomX] = roomIndex;

            // Get base room pattern and apply random rotation
            RoomPattern baseRoom = allRooms[roomIndex];
            int rotation = (rand() % 4) * 90;  // 0, 90, 180, or 270 degrees
            RoomPattern room = rotateRoom(&baseRoom, rotation);

            // Calculate placement position
            int startX = roomX * tileSize + 1;
            int startY = roomY * tileSize + 1;

            // Validate room fits within map bounds
            if (startX + room.width >= totalWidth || startY + room.height >= totalHeight) {
                // Clean up rotated room memory if necessary
                if (rotation != 0 && room.tiles) {
                    for (int y = 0; y < room.height; y++) {
                        free((void *)room.tiles[y]);
                    }
                    free((void *)room.tiles);
                }
                continue;
            }

            // Place room tiles in map
            for (int ry = 0; ry < room.height; ry++) {
                for (int rx = 0; rx < room.width; rx++) {
                    char tile = room.tiles[ry][rx];
                    int val = (tile == '.' ? TILE_EMPTY : TILE_DECORATIVE);
                    mapState->map[startY + ry][startX + rx] = val;
                }
            }

            // Mark walls around the room for visual definition
            for (int dy = -1; dy <= room.height; dy++) {
                for (int dx = -1; dx <= room.width; dx++) {
                    int tx = startX + dx;
                    int ty = startY + dy;
                    if (tx >= 0 && ty >= 0 && tx < totalWidth && ty < totalHeight) {
                        if (mapState->map[ty][tx] == TILE_WALL) {
                            mapState->map[ty][tx] = TILE_DECORATIVE;
                        }
                    }
                }
            }

            // Create horizontal corridors to connect with adjacent rooms
            if (roomX > 0 && previousRooms[roomY][roomX - 1] != -1) {
                int midY = startY + tileSize / 2;
                // Create 3-tile wide corridor for better navigation
                for (int i = -1; i <= 1; i++) {
                    for (int j = 0; j <= 2; j++) {
                        int corridorX = startX - j;
                        int corridorY = midY + i;
                        if (corridorX >= 0 && corridorY >= 0 && 
                            corridorX < totalWidth && corridorY < totalHeight) {
                            mapState->map[corridorY][corridorX] = TILE_EMPTY;
                        }
                    }
                }
            }

            // Create vertical corridors to connect with adjacent rooms
            if (roomY > 0 && previousRooms[roomY - 1][roomX] != -1) {
                int midX = startX + tileSize / 2;
                // Create 3-tile wide corridor for better navigation
                for (int i = -1; i <= 1; i++) {
                    for (int j = 0; j <= 2; j++) {
                        int corridorX = midX + i;
                        int corridorY = startY - j;
                        if (corridorX >= 0 && corridorY >= 0 && 
                            corridorX < totalWidth && corridorY < totalHeight) {
                            mapState->map[corridorY][corridorX] = TILE_EMPTY;
                        }
                    }
                }
            }

            // Update usage counter
            roomUsageCount[roomIndex]++;

            // Clean up rotated room memory if necessary
            if (rotation != 0 && room.tiles) {
                for (int y = 0; y < room.height; y++) {
                    free((void *)room.tiles[y]);
                }
                free((void *)room.tiles);
            }
        }
    }

    // Connect any isolated regions to ensure full map connectivity
    connectIsolatedZones(mapState);

    // Ensure map borders are solid walls for containment
    for (int y = 0; y < mapState->mapHeight; y++) {
        mapState->map[y][0] = TILE_WALL;
        mapState->map[y][mapState->mapWidth - 1] = TILE_WALL;
    }
    for (int x = 0; x < mapState->mapWidth; x++) {
        mapState->map[0][x] = TILE_WALL;
        mapState->map[mapState->mapHeight - 1][x] = TILE_WALL;
    }

    // Place spawn point in first available empty space
    bool spawnPlaced = false;
    for (int y = 1; y < mapState->mapHeight - 1 && !spawnPlaced; y++) {
        for (int x = 1; x < mapState->mapWidth - 1 && !spawnPlaced; x++) {
            if (mapState->map[y][x] == TILE_EMPTY) {
                mapState->map[y][x] = TILE_SPAWN;
                spawnPlaced = true;
            }
        }
    }
    
    if (spawnPlaced) {
        printf("Map generation completed: %dx%d tiles with %dx%d room grid\n",
               totalWidth, totalHeight, numberRoomWidth, numberRoomHeight);
    } else {
        printf("Warning: Map generated but no spawn point could be placed\n");
    }
}
