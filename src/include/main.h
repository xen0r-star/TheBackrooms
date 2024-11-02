#ifndef main_H
#define main_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Inclusions
#define WINDOW_TITLE "ESCAPE THE BACKROOMS"   // Titre de la fenêtre
#define SCREEN_WIDTH 800                // Largeur de la fenêtre
#define SCREEN_HEIGHT 600               // Hauteur de la fenêtre
#define FOV 60.0                        // Champ de vision en degrés
#define textureSize 64                  // Taille des textures
#define numberTextures 4                // Nombre de textures
#define mapSize 50                      // Taille de la map

#define fontFile "font/Roboto-Regular.ttf"
#define backgroundFile "image/background.png"
#define iconFile "image/The_Backrooms_logo.png"


// Structure
typedef struct {
    float x, y;
    float angle;
} Player;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    char *text;
} Button;

typedef struct{
    int r, g, b, a;
} Color;



// Variables globales
extern SDL_Renderer *renderer;
extern SDL_Window *window;
extern const Uint8 *keystate;
extern SDL_GameController *controller;
extern TTF_Font *font;
extern SDL_Texture *message;
extern Uint32 *screenBuffers;
extern SDL_Texture* screenBuffersTexture;
extern Uint32 **textureBuffers;
extern SDL_Texture *backgroundTexture;

extern bool running;
extern bool showMap;
extern bool showState;
extern bool showTextures;
extern bool colision;
extern int displayMenu;

extern Button playButton;
extern Button achievementsButton;
extern Button settingsButton;
extern Button exitButton;
extern Button resumeGameButton;
extern Button extiGameButton;

extern Player player;
extern float rotateSpeed;
extern float moveSpeed;
extern float sensitivity;
extern int selectFrame;

extern float playerRotateSpeed;
extern float playerMoveSpeed;

extern int mapWidth;
extern int mapHeight;
extern int map[mapSize][mapSize];
extern int mapDiscovered[mapSize][mapSize];

extern float fps;
extern clock_t startTime, previousTime;

#endif