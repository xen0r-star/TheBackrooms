#ifndef handle_H
#define handle_H

#include "main.h"

extern void keyboardDown(GameState *state, const SDL_Event event);
extern void controllerDown(GameState *state, const SDL_Event event);

extern void mouseHandle(GameState *state, const SDL_Event event);

extern void keyboardInput(GameState *state);
extern void controllerInput(GameState *state);

#endif