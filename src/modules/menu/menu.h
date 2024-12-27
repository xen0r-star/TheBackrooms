#ifndef menu_H
#define menu_H

#include "../../main.h"
#include "../map/map.h"
#include "../utils/utils.h"

typedef enum {
    BUTTON_NORMAL,
    BUTTON_SELECTED,
    BUTTON_FOCUS
} ButtonType;

typedef enum {
    MENU_NONE,
    MENU_MAIN,
    MENU_LOAD,
    MENU_ACHIEVEMENTS,
    MENU_SETTINGS,
    MENU_BREAK
} MenuType;

extern int initializationMenu(GameState *state);

extern bool clickedButton(Button button, int mouseX, int mouseY);
extern void drawMenu(GameState *state);
extern void handleButtons(AppState *appState, int mouseX, int mouseY, int buttonCount, ...);

void drawButton(AppState *appState, Button button, ButtonType type);

#endif