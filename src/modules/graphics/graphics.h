#ifndef graphics_H
#define render_H

#include "../../main.h"
#include "../core/core.h"
#include "../entities/entity.h"

extern void renderScene(GameState *state);

extern void glitchEffect(const AppState appState, GraphicsBuffers *graphicsBuffers, int speed);

extern int initializationTextures(GameState *state);
extern int initializationScreen(GameState *state);

extern int initializationWindow(GameState *state);
extern int closeWindow(GameState *state);
extern bool hasWindowResize(AppState *appState);

#endif