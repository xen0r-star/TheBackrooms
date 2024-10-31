#ifndef utils_H
#define utils_H

#include "main.h"

extern SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filename);

extern int renderText(int x, int y, int w, int h, const char *text, SDL_Color color);

extern void drawButton(SDL_Renderer *renderer, Button button);
extern bool clickedButton(Button button, int mouseX, int mouseY);

extern void showStateInterface();
extern void showMapInterface();

extern void itemFrame(int selectFrame);

#endif