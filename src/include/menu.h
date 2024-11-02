#ifndef menu_H
#define menu_H

#include "main.h"

#define BUTTON_NORMAL 0
#define BUTTON_SELECTED 1
#define BUTTON_FOCUS 2

extern bool clickedButton(Button button, int mouseX, int mouseY);
extern void menu(int mouseX, int mouseY, int buttonCount, ...);

#endif