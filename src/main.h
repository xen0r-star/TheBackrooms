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
#define NUMBER_TEXTURES 5                           // Nombre de textures
#define MAP_SIZE_LEVEL0 6                           // Nombre de salles dans le niveau 0

#define FONT_ROBOTO "font/Roboto-Regular.ttf"       // Fichier de police
#define FONT_JERSEY "font/Jersey25-Regular.ttf"     // Fichier de police
#define ICON_FILE "image/The_Backrooms_logo.png"    // Fichier d'icône



// Enum
typedef enum {
    TEXT_XS,        // 26
    TEXT_S,         // 28
    TEXT_M,         // 32
    TEXT_L,         // 36
    TEXT_XL,        // 48
    TEXT_XXL,       // 72
    TEXT_TITLE,     // 94
    TEXT_TYPE_COUNT 
} TextType;

typedef enum {
    LEFT,
    CENTER,
    RIGHT
} Alignment;

typedef enum {
    BUTTON_NORMAL,
    BUTTON_SELECTED,
    BUTTON_HOVER
} ButtonType;

typedef enum {
    MENU_NONE,
    MENU_MAIN,
    MENU_LOAD,
    MENU_ACHIEVEMENTS,
    MENU_SETTINGS,
    MENU_BREAK
} MenuType;

typedef enum {
    BACKGROUND_MENU,
    BACKGROUND_GAME,
    BAKCGROUND_TYPE_COUNT
} BackgroundType;



// Structure
typedef struct {
    float x, y;
    float angle;
} Player;

typedef struct {
    double x, y;
    int texture_id;
} Sprite;

typedef struct{
    int r, g, b, a;
} Color;

typedef struct {
    int x, y;
    int w, h;
} Rect;

typedef struct {
    Rect rect;
    Color color;
    char *text;
} Button;

typedef struct {
    char *family;
    int size;
} Font;




typedef struct {
    int screenWidth;
    int screenHeight;
    bool running;

    SDL_Renderer       *renderer;
    SDL_Window         *window;
    const Uint8        *keystate;
    SDL_GameController *controller;
    TTF_Font           *text[TEXT_TYPE_COUNT];

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
    Sprite* sprites;
    int numSprites;
} EntityState;


typedef struct {
    int displayMenu;
    BackgroundType backgroundType;
    int scrollOffset;

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
    Button returnButton;
} MenuState;

typedef struct {
    int mapWidth;
    int mapHeight;
    int **map;
    int **mapDiscovered;
    int typeLaunchGame;
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
    EntityState entityState;
    MenuState menu;
    MapState mapState;
    GraphicsBuffers graphics;
    Settings settings;
} GameState;


extern GameState gameState;


#endif