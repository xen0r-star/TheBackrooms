#ifndef handle_H
#define handle_H

#include "main.h"

extern void keyboardDown(const SDL_Event event);
extern void controllerDown(const SDL_Event event);

extern void mouseMotion(const SDL_Event event);

extern void keyboardInput();
extern void controllerInput();

#endif