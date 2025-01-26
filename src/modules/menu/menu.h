#ifndef menu_H
#define menu_H

#include "../../main.h"
#include "../map/map.h"
#include "../utils/utils.h"


extern int initializationMenu(GameState *state);

extern bool clickedButton(Button button, int mouseX, int mouseY);
extern void drawMenu(GameState *state);
extern void handleButtons(AppState *appState, int mouseX, int mouseY, int buttonCount, ...);

void drawButton(AppState *appState, Button button, ButtonType type);
void background(GameState *state);
void header(GameState *state, char *Title);

#endif