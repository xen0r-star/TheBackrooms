#ifndef utils_H
#define utils_H

#include "main.h"

extern int renderText(int x, int y, int w, int h, const char *text, SDL_Color color);

extern void showStateInterface();
extern void showMapInterface();

extern void itemFrame(int selectFrame);

#endif