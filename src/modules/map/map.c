#include "map.h"


RoomPattern allRooms[7] = {
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

#define MAX_REGIONS 100
#define MIN_REGION_SIZE 10



RoomPattern rotateRoom(RoomPattern *room, int rotation) {
    RoomPattern result;
    if (rotation == 0) return *room;

    int w = room->width;
    int h = room->height;

    if (rotation == 90 || rotation == 270) {
        result.width = h;
        result.height = w;
    } else {
        result.width = w;
        result.height = h;
    }

    const char **tiles = malloc(result.height * sizeof(char*));
    for (int y = 0; y < result.height; y++) {
        tiles[y] = malloc(result.width + 1); // +1 for '\0'
    }

    for (int y = 0; y < result.height; y++) {
        for (int x = 0; x < result.width; x++) {
            char c;
            switch (rotation) {
                case 90:
                    c = room->tiles[w - 1 - x][y];
                    break;
                case 180:
                    c = room->tiles[h - 1 - y][w - 1 - x];
                    break;
                case 270:
                    c = room->tiles[x][h - 1 - y];
                    break;
                default:
                    c = room->tiles[y][x];
                    break;
            }
            ((char *)tiles[y])[x] = c;
        }
        ((char *)tiles[y])[result.width] = '\0';
    }

    result.tiles = tiles;
    return result;
}

void floodFill(int **map, int width, int height, int **regionMap, int x, int y, int regionId) {
    if (x < 0 || y < 0 || x >= width || y >= height)
        return;
    if (map[y][x] != 0 || regionMap[y][x] != -1)
        return;

    regionMap[y][x] = regionId;

    floodFill(map, width, height, regionMap, x + 1, y, regionId);
    floodFill(map, width, height, regionMap, x - 1, y, regionId);
    floodFill(map, width, height, regionMap, x, y + 1, regionId);
    floodFill(map, width, height, regionMap, x, y - 1, regionId);
}

void connectIsolatedZones(MapState *mapState) {
    int width = mapState->mapWidth;
    int height = mapState->mapHeight;

    int **regionMap = malloc(height * sizeof(int *));
    for (int y = 0; y < height; y++) {
        regionMap[y] = malloc(width * sizeof(int));
        for (int x = 0; x < width; x++) {
            regionMap[y][x] = -1;
        }
    }

    int regionCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (mapState->map[y][x] == 0 && regionMap[y][x] == -1) {
                if (regionCount < MAX_REGIONS) {
                    floodFill(mapState->map, width, height, regionMap, x, y, regionCount);
                    regionCount++;
                }
            }
        }
    }

    // Relier toutes les régions > 0 à la région 0
    for (int r = 1; r < regionCount; r++) {
        // Vérifier la taille de la région r
        int regionSize = 0;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (regionMap[y][x] == r)
                    regionSize++;
            }
        }

        // Trop petite : ignorer
        if (regionSize < MIN_REGION_SIZE)
            continue;

        int minDist = 99999;
        int ax = -1, ay = -1, bx = -1, by = -1;

        for (int y1 = 0; y1 < height; y1++) {
            for (int x1 = 0; x1 < width; x1++) {
                if (regionMap[y1][x1] != r) continue;

                for (int y2 = 0; y2 < height; y2++) {
                    for (int x2 = 0; x2 < width; x2++) {
                        if (regionMap[y2][x2] != 0) continue;

                        int dist = abs(x1 - x2) + abs(y1 - y2);
                        if (dist < minDist) {
                            minDist = dist;
                            ax = x1; ay = y1;
                            bx = x2; by = y2;
                        }
                    }
                }
            }
        }

        // === Couloir sinueux : détour pivot (X, Y) ===
        int pivotX = ax + (rand() % (abs(bx - ax) + 1)) * ((bx > ax) ? 1 : -1);
        int pivotY = by;

        // Axe horizontal vers pivotX
        int cx = ax;
        while (cx != pivotX) {
            mapState->map[ay][cx] = 0;
            cx += (pivotX > cx) ? 1 : -1;
        }

        // Axe vertical vers pivotY
        int cy = ay;
        while (cy != pivotY) {
            mapState->map[cy][pivotX] = 0;
            cy += (pivotY > cy) ? 1 : -1;
        }

        // Axe horizontal final
        cx = pivotX;
        while (cx != bx) {
            mapState->map[by][cx] = 0;
            cx += (bx > cx) ? 1 : -1;
        }
    }

    for (int y = 0; y < height; y++) free(regionMap[y]);
    free(regionMap);
}


void generateMap(MapState *mapState, int numberRoomWidth, int numberRoomHeight) {
    if (mapState->map != NULL) {
        for (int i = 0; i < mapState->mapHeight; i++)
            free(mapState->map[i]);
        free(mapState->map);
    }

    if (mapState->mapDiscovered != NULL) {
        for (int i = 0; i < mapState->mapHeight; i++)
            free(mapState->mapDiscovered[i]);
        free(mapState->mapDiscovered);
    }

    int tileSize = 12;
    int totalWidth = numberRoomWidth * tileSize + 1;
    int totalHeight = numberRoomHeight * tileSize + 1;
    mapState->mapWidth = totalWidth;
    mapState->mapHeight = totalHeight;

    mapState->map = malloc(totalHeight * sizeof(int *));
    mapState->mapDiscovered = malloc(totalHeight * sizeof(int *));
    for (int y = 0; y < totalHeight; y++) {
        mapState->map[y] = malloc(totalWidth * sizeof(int));
        mapState->mapDiscovered[y] = malloc(totalWidth * sizeof(int));
        for (int x = 0; x < totalWidth; x++) {
            mapState->map[y][x] = 1;
            mapState->mapDiscovered[y][x] = 0;
        }
    }

    srand((unsigned int)time(NULL));
    int roomUsageCount[7] = {0};

    int previousRooms[numberRoomHeight][numberRoomWidth];
    for (int y = 0; y < numberRoomHeight; y++)
        for (int x = 0; x < numberRoomWidth; x++)
            previousRooms[y][x] = -1;

    for (int roomY = 0; roomY < numberRoomHeight; roomY++) {
        for (int roomX = 0; roomX < numberRoomWidth; roomX++) {
            if (rand() % 10 == 0) continue;

            int roomIndex;
            int tries = 0;
            do {
                roomIndex = rand() % 7;

                // Vérifie la limite d'occurrence
                RoomPattern *tryRoom = &allRooms[roomIndex];
                if (tryRoom->maxOccurrences != -1 &&
                    roomUsageCount[roomIndex] >= tryRoom->maxOccurrences) {
                    tries++;
                    continue;
                }

                // Évite de répéter une salle identique à gauche ou en haut
                if ((roomX > 0 && previousRooms[roomY][roomX - 1] == roomIndex) ||
                    (roomY > 0 && previousRooms[roomY - 1][roomX] == roomIndex)) {
                    tries++;
                    continue;
                }

                break;
            } while (tries < 20);

            if (tries >= 20) continue;

            previousRooms[roomY][roomX] = roomIndex;

            RoomPattern baseRoom = allRooms[roomIndex];
            int rotation = (rand() % 4) * 90;
            RoomPattern room = rotateRoom(&baseRoom, rotation);

            int startX = roomX * tileSize + 1;
            int startY = roomY * tileSize + 1;

            if (startX + room.width >= totalWidth || startY + room.height >= totalHeight)
                continue;

            for (int ry = 0; ry < room.height; ry++) {
                for (int rx = 0; rx < room.width; rx++) {
                    char tile = room.tiles[ry][rx];
                    int val = (tile == '.' ? 0 : 2);
                    mapState->map[startY + ry][startX + rx] = val;
                }
            }

            // Marquer les murs autour de la salle
            for (int dy = -1; dy <= room.height; dy++) {
                for (int dx = -1; dx <= room.width; dx++) {
                    int tx = startX + dx;
                    int ty = startY + dy;
                    if (tx >= 0 && ty >= 0 && tx < totalWidth && ty < totalHeight) {
                        if (mapState->map[ty][tx] == 1)
                            mapState->map[ty][tx] = 2;
                    }
                }
            }

            // === Couloirs horizontaux ===
            if (roomX > 0 && previousRooms[roomY][roomX - 1] != -1) {
                int midY = startY + tileSize / 2;
                for (int i = -1; i <= 1; i++)
                    for (int j = 0; j <= 2; j++)
                        mapState->map[midY + i][startX - j] = 0;
            }

            // === Couloirs verticaux ===
            if (roomY > 0 && previousRooms[roomY - 1][roomX] != -1) {
                int midX = startX + tileSize / 2;
                for (int i = -1; i <= 1; i++)
                    for (int j = 0; j <= 2; j++)
                        mapState->map[startY - j][midX + i] = 0;
            }

            // Incrémentation du compteur d'utilisation
            roomUsageCount[roomIndex]++;

            // Libération mémoire rotation (si rotation)
            if (rotation != 0) {
                for (int y = 0; y < room.height; y++)
                    free((void *)room.tiles[y]);
                free((void *)room.tiles);
            }
        }
    }

    connectIsolatedZones(mapState);

    // Marquer les bords du tableau comme murs
    for (int y = 0; y < mapState->mapHeight; y++) {
        mapState->map[y][0] = 1;
        mapState->map[y][mapState->mapWidth - 1] = 1;
    }
    for (int x = 0; x < mapState->mapWidth; x++) {
        mapState->map[0][x] = 1;
        mapState->map[mapState->mapHeight - 1][x] = 1;
    }


    // Placer le point de spawn
    bool spawnPlaced = false;
    for (int y = 1; y < mapState->mapHeight - 1 && !spawnPlaced; y++) {
        for (int x = 1; x < mapState->mapWidth - 1 && !spawnPlaced; x++) {
            if (mapState->map[y][x] == 0) {
                mapState->map[y][x] = 9;  // '9' représente le point de spawn
                spawnPlaced = true;
            }
        }
    }
}
