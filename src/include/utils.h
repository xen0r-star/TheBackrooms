#ifndef utils_H
#define utils_H

#include "main.h"

extern void calculateFPS(GameState *state);

extern int renderText(GameState *state, int x, int y, int w, int h, const char *text, SDL_Color color);

extern void showStateInterface(GameState *state);
extern void showMapInterface(GameState *state);

extern void itemFrame(GameState *state, int selectFrame);

#endif