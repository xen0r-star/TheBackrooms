#ifndef menu_H
#define menu_H

#include "main.h"

#define BUTTON_NORMAL 0
#define BUTTON_SELECTED 1
#define BUTTON_FOCUS 2

#define MENU_NONE 0
#define MENU_MAIN 1
#define MENU_LOAD 2
#define MENU_ACHIEVEMENTS 3
#define MENU_SETTINGS 4
#define MENU_BREAK 5

extern int initializationMenu();
extern bool clickedButton(Button button, int mouseX, int mouseY);
extern void drawMenu();

#endif