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
// #define screenWidth 800                // Largeur de la fenêtre
// #define screenHeight 600               // Hauteur de la fenêtre
#define FOV 60.0                        // Champ de vision en degrés
#define TEXTURE_SIZE 64                  // Taille des textures
#define NUMBER_TEXTURES 4                // Nombre de textures
#define MAP_SIZE 50                      // Taille de la map

#define FONT_FILE "font/Roboto-Regular.ttf"
#define BACKGROUND_FILE "image/background.png"
#define ICON_FILE "image/The_Backrooms_logo.png"


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




typedef struct {
    int screenWidth;
    int screenHeight;
    bool running;

    SDL_Renderer       *renderer;
    SDL_Window         *window;
    const Uint8        *keystate;
    SDL_GameController *controller;
    TTF_Font           *font;
    SDL_Texture        *message;

    float fps;
    clock_t startTime;
    clock_t previousTime;
} AppState;


typedef struct {
    Player player;
    float rotateSpeed;
    float moveSpeed;

    float sensitivity;
    float playerRotateSpeed;
    float playerMoveSpeed;
    int selectFrame;

    bool showMap;
    bool showState;
    bool showTextures;
    bool collision;
} PlayerState;


typedef struct {
    int displayMenu;
    SDL_Texture *backgroundTexture;

    Button playButton;
    Button achievementsButton;
    Button settingsButton;
    Button exitButton;
    Button resumeGameButton;
    Button exitGameButton;
    Button loadGame1;
    Button loadGame2;
    Button loadGame3;
    Button launchGame;
} MenuState;

typedef struct {
    int mapWidth;
    int mapHeight;
    int map[MAP_SIZE][MAP_SIZE];
    int mapDiscovered[MAP_SIZE][MAP_SIZE];
    int typeLaunchGame[3];
} MapState;

typedef struct {
    Uint32      *screenBuffers;
    SDL_Texture *screenBuffersTexture;
    Uint32     **textureBuffers;
} GraphicsBuffers;



typedef struct {
    AppState app;
    PlayerState playerState;
    MenuState menu;
    MapState mapState;
    GraphicsBuffers graphics;
} GameState;


extern GameState gameState;


// Variables globales
// extern int screenWidth;
// extern int screenHeight;

// extern SDL_Renderer *renderer;
// extern SDL_Window *window;
// extern const Uint8 *keystate;
// extern SDL_GameController *controller;
// extern TTF_Font *font;
// extern SDL_Texture *message;
// extern Uint32 *screenBuffers;
// extern SDL_Texture* screenBuffersTexture;
// extern Uint32 **textureBuffers;
// extern SDL_Texture *backgroundTexture;

// extern bool running;
// extern bool showMap;
// extern bool showState;
// extern bool showTextures;
// extern bool colision;
// extern int displayMenu;

// extern Button playButton;
// extern Button achievementsButton;
// extern Button settingsButton;
// extern Button exitButton;
// extern Button resumeGameButton;
// extern Button extiGameButton;
// extern Button loadGame1;
// extern Button loadGame2;
// extern Button loadGame3;
// extern Button launchGame;

// extern Player player;
// extern float rotateSpeed;
// extern float moveSpeed;
// extern float sensitivity;
// extern int selectFrame;

// extern float playerRotateSpeed;
// extern float playerMoveSpeed;

// extern int mapWidth;
// extern int mapHeight;
// extern int map[MAP_SIZE][MAP_SIZE];
// extern int mapDiscovered[MAP_SIZE][MAP_SIZE];

// extern int typeLaunchGame[3];

// extern float fps;
// extern clock_t startTime, previousTime;

#endif