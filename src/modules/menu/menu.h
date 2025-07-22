#ifndef menu_H
#define menu_H

#include "../../main.h"
#include "../map/map.h"
#include "../data/data.h"
#include "../utils/utils.h"


extern int initializationMenu(GameState *state);

extern bool clickedButton(Button button, int mouseX, int mouseY);
extern void drawMenu(GameState *state);
extern void handleButtons(AppState *appState, int mouseX, int mouseY, int buttonCount, ...);
extern void handleMenuButtons(GameState *state, int mouseX, int mouseY);

void drawButton(AppState *appState, Button button, ButtonType type);
void background(GameState *state, BackgroundType backgroundType);

#endif