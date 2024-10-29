#ifndef keyboard_H
#define keyboard_H

#include "main.h"

extern void keyboardDown(const SDL_Event event);
extern void keyboardInput(const Uint8 *keystate);

#endif