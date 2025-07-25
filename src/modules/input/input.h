#ifndef input_H
#define input_H

#include "../../main.h"
#include "../menu/menu.h"
#include "../graphics/graphics.h"


extern void keyboardDown(GameState *state, const SDL_Event event);
extern void controllerDown(GameState *state, const SDL_Event event);

extern void mouseHandle(GameState *state, const SDL_Event event);

extern void keyboardInput(GameState *state);
extern void controllerInput(GameState *state);

#endif