#include "main.h"
#include "initialization.h"
#include "handle.h"
#include "render.h"
#include "map.h"
#include "data.h"
#include "menu.h"
#include "utils.h"



SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
const Uint8 *keystate;
SDL_GameController *controller = NULL;
TTF_Font *font = NULL;
SDL_Texture *message = NULL;
Uint32 *screenBuffers;
SDL_Texture* screenBuffersTexture;
Uint32 **textureBuffers;
SDL_Texture *backgroundTexture = NULL;

Button playButton;
Button achievementsButton;
Button settingsButton;
Button exitButton;
Button resumeGameButton;
Button extiGameButton;

bool running = true;
bool showMap = false;
bool showState = false;
bool showTextures = true;
bool colision = true;
int displayMenu = 1;

Player player = {4.0, 4.0, 0.0};
float rotateSpeed = 150.0;
float moveSpeed = 3.0;
float sensitivity = 0.2;
int selectFrame = 1;

float playerRotateSpeed, playerMoveSpeed;

int mapWidth = mapSize;
int mapHeight = mapSize;
int map[mapSize][mapSize];
int mapDiscovered[mapSize][mapSize];

float fps;
clock_t startTime, previousTime;



int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation de la fenêtre
    initialization();

    backgroundTexture = IMG_LoadTexture(renderer, backgroundFile);

    // Création menu
    playButton = (Button){
        {(SCREEN_WIDTH - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    achievementsButton = (Button){
        {(SCREEN_WIDTH - 400) / 2, (275 + 1 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Succès"
    };

    settingsButton = (Button){
        {(SCREEN_WIDTH - 400) / 2, (275 + 2 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Paramètres"
    };

    exitButton = (Button){
        {(SCREEN_WIDTH - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    resumeGameButton = (Button){
        {(SCREEN_WIDTH - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Reprendre"
    };

    extiGameButton = (Button){
        {(SCREEN_WIDTH - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter et sauvegarder"
    };


    // création de la map
    generateMap(map, mapSize, mapSize, 5);
    for (int y = 0; y < mapSize; y++) {
        for (int x = 0; x < mapSize; x++) {
            mapDiscovered[y][x] = 0;
        }
    }


    // Chargement textures
    const char* texturePaths[numberTextures] = {
        "textures/breadMat.png",
        "textures/breadMat.png",

        "textures/floor.png",
        "textures/ceiling.png",
    };

    textureBuffers = (Uint32**)malloc(numberTextures * sizeof(Uint32*));
    for (int i = 0; i < numberTextures; i++) {
        SDL_Surface* surface = IMG_Load(texturePaths[i]);
        textureBuffers[i] = (Uint32*)malloc(textureSize * textureSize * sizeof(Uint32));
    
        for (int y = 0; y < textureSize; y++) {
            for (int x = 0; x < textureSize; x++) {
                Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
                pixel = ((pixel & 0xFF)         << 24) // RGBA => ABGR
                    |   (((pixel >> 8 ) & 0xFF) << 16) 
                    |   (((pixel >> 16) & 0xFF) << 8 ) 
                    |   ((pixel  >> 24) & 0xFF);
                textureBuffers[i][y * textureSize + x] = pixel;
            }
        }

        SDL_FreeSurface(surface);
    }


    // Création de la texture de rendu
    screenBuffersTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT
    );

    screenBuffers = (Uint32 *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));


    // Boucle principale
    SDL_Event event;
    startTime = clock(), previousTime = clock();
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            mouseHandle(event);
            keyboardDown(event);

            if (controller != NULL) controllerDown(event);
        }


        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        if (displayMenu != 0) {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (displayMenu == 1) {
                SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
                menu(mouseX, mouseY, 4, playButton, achievementsButton, settingsButton, exitButton);
            } else if (displayMenu == 2) { // Jouer
                displayMenu = 0;
                
            } else if (displayMenu == 3) { // Succès
                SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

                int scrollOffset = 0;
                const int scrollSpeed = 20;
                const int successCount = 10;
                const int contentHeight = successCount * 125 + (successCount - 1) * 20;
                const int viewHeight = SCREEN_HEIGHT - 50;


                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
                for (int i = 0; i < successCount; i++) {
                    SDL_Rect block = {
                        50, 
                        50 + 125 * i + 20 * i - scrollOffset, 
                        SCREEN_WIDTH - 100 - 15, 
                        125
                    };
                    SDL_RenderFillRect(renderer, &block);
                }

                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 240);
                SDL_Rect scrollbar = {SCREEN_WIDTH - 30, 50, 10, viewHeight - 10};
                SDL_RenderFillRect(renderer, &scrollbar);

                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                scrollbar = (SDL_Rect){
                    SCREEN_WIDTH - 30, 
                    50 + ((scrollOffset * viewHeight) / contentHeight), 
                    10, 
                    (viewHeight * viewHeight) / contentHeight
                };
                SDL_RenderFillRect(renderer, &scrollbar);

            } else if (displayMenu == 4) { // Paramètres
                
            } else if (displayMenu == 5) { // Pause
                SDL_UpdateTexture(screenBuffersTexture, NULL, screenBuffers, SCREEN_WIDTH * sizeof(Uint32));
                SDL_RenderCopy(renderer, screenBuffersTexture, NULL, NULL);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
                SDL_Rect block = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderFillRect(renderer, &block);

                menu(mouseX, mouseY, 4, resumeGameButton, achievementsButton, settingsButton, extiGameButton);
            }

        } else {
            SDL_SetRelativeMouseMode(SDL_TRUE);

            // Calcul du FPS
            fps = (double) (clock() - startTime) / CLOCKS_PER_SEC;
            startTime = clock();

            playerMoveSpeed = moveSpeed * fps;
            playerRotateSpeed = rotateSpeed * fps;
            fps = (int)(1. / fps);


            keyboardInput();
            if (controller != NULL) controllerInput();

            renderScene();
            itemFrame(selectFrame);

            if (showState) showStateInterface();
            if(showMap) showMapInterface();
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);
    }


    // Fermeture de la fenêtre
    close();
    return 0;
}
