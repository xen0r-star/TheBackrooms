#include "main.h"
#include "initialization.h"
#include "handle.h"
#include "render.h"
#include "map.h"
#include "data.h"
#include "utils.h"



SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
Uint8 *keystate = NULL;
SDL_GameController *controller = NULL;
TTF_Font *font = NULL;
SDL_Texture *message = NULL;
Uint32 *screenBuffers;
SDL_Texture* screenBuffersTexture;
Uint32 **textureBuffers;
SDL_Texture *backgroundTexture = NULL;
SDL_Cursor *pointerCursor = NULL;
SDL_Cursor *defaultCursor = NULL;

bool running = true;
bool showMap = false;
bool showState = false;
bool showTextures = true;
bool colision = true;
int menu = 1;

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


    SDL_Cursor *pointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Cursor *defaultCursor = SDL_GetDefaultCursor();

    backgroundTexture = IMG_LoadTexture(renderer, backgroundFile);

    // Création menu
    Button playButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    Button achievementsButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 1 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Succès"
    };

    Button settingsButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 2 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Paramètres"
    };

    Button exitButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    Button resumeGameButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Reprendre"
    };

    Button extiGameButton = {
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
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && menu != 0) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (clickedButton(achievementsButton, mouseX, mouseY))  menu = 3;
                else if (clickedButton(settingsButton, mouseX, mouseY))      menu = 4;

                if (menu == 1) {
                    if (clickedButton(playButton, mouseX, mouseY)) {
                        menu = 2;
                    } else if (clickedButton(achievementsButton, mouseX, mouseY)) {
                        menu = 3;
                    } else if (clickedButton(settingsButton, mouseX, mouseY)) {
                        menu = 4;
                    } else if (clickedButton(exitButton, mouseX, mouseY)) {
                        running = false;
                    }
                } else if (menu == 5) {
                    if (clickedButton(resumeGameButton, mouseX, mouseY)) {
                        menu = 0;
                    } else if (clickedButton(achievementsButton, mouseX, mouseY)) { 
                        menu = 3;
                    } else if (clickedButton(settingsButton, mouseX, mouseY)) {     
                        menu = 4;
                    } else if (clickedButton(extiGameButton, mouseX, mouseY)) {
                        menu = 1;
                        saveData("Save1");
                    }
                }
            }

            mouseMotion(event);
            keyboardDown(event);

            if (controller != NULL) controllerDown(event);
        }


        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bool cursorChanged = false;

        if (menu != 0) {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (menu == 1) {
                playButton.color.a = 127;
                achievementsButton.color.a = 127;
                settingsButton.color.a = 127;
                exitButton.color.a = 127;

                SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

                if (clickedButton(playButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    playButton.color.a = 225;
                } else if (clickedButton(achievementsButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    achievementsButton.color.a = 225;
                } else if (clickedButton(settingsButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    settingsButton.color.a = 225;
                } else if (clickedButton(exitButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    exitButton.color.a = 225;
                }

                drawButton(renderer, playButton);
                drawButton(renderer, achievementsButton);
                drawButton(renderer, settingsButton);
                drawButton(renderer, exitButton);
            } else if (menu == 2) { // Jouer
                menu = 0;
                
            } else if (menu == 3) { // Succès
                
            } else if (menu == 4) { // Paramètres
                
            } else if (menu == 5) { // Pause
                resumeGameButton.color.a = 127;
                achievementsButton.color.a = 127;
                settingsButton.color.a = 127;
                extiGameButton.color.a = 127;

                SDL_UpdateTexture(screenBuffersTexture, NULL, screenBuffers, SCREEN_WIDTH * sizeof(Uint32));
                SDL_RenderCopy(renderer, screenBuffersTexture, NULL, NULL);

                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
                SDL_Rect block = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderFillRect(renderer, &block);

                if (clickedButton(resumeGameButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    resumeGameButton.color.a = 225;
                } else if (clickedButton(achievementsButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    achievementsButton.color.a = 225;
                } else if (clickedButton(settingsButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    settingsButton.color.a = 225;
                } else if (clickedButton(extiGameButton, mouseX, mouseY)) {
                    SDL_SetCursor(pointerCursor);
                    cursorChanged = true;
                    extiGameButton.color.a = 225;
                }

                drawButton(renderer, resumeGameButton);
                drawButton(renderer, achievementsButton);
                drawButton(renderer, settingsButton);
                drawButton(renderer, extiGameButton);
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

        if (!cursorChanged) {
            SDL_SetCursor(defaultCursor);
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);
    }


    // Fermeture de la fenêtre
    close();
    return 0;
}
