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



#define WINDOW_TITLE "ESCAPE THE BACKROOMS"         // Titre de la fenêtre
#define TEXTURE_SIZE 64                             // Taille des textures
#define NUMBER_TEXTURES 4                           // Nombre de textures
#define MAP_SIZE 50                                 // Taille de la map

#define FONT_FILE "font/Roboto-Regular.ttf"         // Fichier de police
#define BACKGROUND_FILE "image/background.png"      // Fichier de fond menu
#define ICON_FILE "image/The_Backrooms_logo.png"    // Fichier d'icône


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
    float sensitivity;
    float fov;
    float volume;
} Settings;


typedef struct {
    AppState app;
    PlayerState playerState;
    MenuState menu;
    MapState mapState;
    GraphicsBuffers graphics;
    Settings settings;
} GameState;


extern GameState gameState;


#endif