#ifndef utils_H
#define utils_H

#include "../../main.h"
#include "../map/map.h"
#include "../graphics/graphics.h"

extern void calculateFPS(AppState *appState, PlayerState *playerState);

extern int renderText(AppState *appState, int x, int y, int w, int h, const char *text, SDL_Color color);

extern void showStateInterface(AppState *appState, const PlayerState playerState);
extern void showMapInterface(const AppState appState, const MapState mapState, const PlayerState playerState);

extern void itemFrame(const AppState appState, int selectFrame);

extern void backgroundMenu();

#endif