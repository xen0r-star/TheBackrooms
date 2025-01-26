#ifndef utils_H
#define utils_H

#include "../../main.h"
#include "../map/map.h"
#include "../graphics/graphics.h"

extern void calculateFPS(AppState *appState, PlayerState *playerState);

extern int renderText(AppState *appState, Alignment alignment, int x, int y, const char *text, Color color, TextType type);

extern void showStateInterface(AppState *appState, const PlayerState playerState);
extern void showMapInterface(const AppState appState, const MapState mapState, const PlayerState playerState);

extern void itemFrame(const AppState appState, int selectFrame);

#endif