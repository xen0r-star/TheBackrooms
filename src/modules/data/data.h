#ifndef data_H
#define data_H

#include "../../main.h"
#include "../map/map.h"

extern int saveData(GameState *state, const char *nameSave);
extern int readData(GameState *state, const char *nameSave);

#endif