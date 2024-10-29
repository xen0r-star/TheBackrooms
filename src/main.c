#include "main.h"
#include "window.h"
#include "keyboard.h"
#include "render.h"
#include "map.h"
#include "utils.h"



SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
TTF_Font *font = NULL;
SDL_Texture *message = NULL;
Uint32 *screenBuffers;
SDL_Texture* screenBuffersTexture;
Uint32 **textureBuffers;
SDL_Texture *backgroundTexture = NULL;
SDL_Cursor *pointerCursor = NULL;
SDL_Cursor *defaultCursor = NULL;

bool running = true;
bool menu = true;
bool showMap = false;
bool showRays = false;
bool showState = false;
bool showTextures = true;
bool colision = true;

Player player = {4.0, 4.0, 0.0};
float rotateSpeed = 5.0;
float moveSpeed = 0.1;

int mapWidth = mapSize;
int mapHeight = mapSize;
int map[mapSize][mapSize];
int mapWallSize;

double fps;



int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // Initialisation de la fenêtre
    createWindow();


    SDL_Cursor *pointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Cursor *defaultCursor = SDL_GetDefaultCursor();

    backgroundTexture = IMG_LoadTexture(renderer, backgroundFile);

    // Création menu
    Button playButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 0 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Jouer"
    };

    Button settingsButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 1 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Paramètres"
    };

    Button achievementsButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 2 * (50 + 20)), 400, 50},
        {137, 136, 113, 127},
        "Succès"
    };

    Button exitButton = {
        {(SCREEN_WIDTH - 400) / 2, (275 + 3 * (50 + 20)), 400, 50}, 
        {137, 136, 113, 127},
        "Quitter"
    };


    // création de la map
    generateMap(map, mapSize, mapSize, 5);
    mapWallSize = (SCREEN_HEIGHT * 0.4) / mapHeight;


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
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    clock_t startTime = clock(), previousTime = clock();
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (clickedButton(playButton, mouseX, mouseY)) {
                    menu = false;
                } else if (clickedButton(settingsButton, mouseX, mouseY)) {
                    
                } else if (clickedButton(achievementsButton, mouseX, mouseY)) {
                    
                } else if (clickedButton(exitButton, mouseX, mouseY)) {
                    running = false;
                }
            }

            keyboardDown(event);
        }


        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bool cursorChanged = false;
        playButton.color.a = 127;
        settingsButton.color.a = 127;
        achievementsButton.color.a = 127;
        exitButton.color.a = 127;

        if (menu) {
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (clickedButton(playButton, mouseX, mouseY)) {
                SDL_SetCursor(pointerCursor);
                cursorChanged = true;
                playButton.color.a = 225;
            } else if (clickedButton(settingsButton, mouseX, mouseY)) {
                SDL_SetCursor(pointerCursor);
                cursorChanged = true;
                settingsButton.color.a = 225;
            } else if (clickedButton(achievementsButton, mouseX, mouseY)) {
                SDL_SetCursor(pointerCursor);
                cursorChanged = true;
                achievementsButton.color.a = 225;
            } else if (clickedButton(exitButton, mouseX, mouseY)) {
                SDL_SetCursor(pointerCursor);
                cursorChanged = true;
                exitButton.color.a = 225;
            }

            drawButton(renderer, playButton);
            drawButton(renderer, settingsButton);
            drawButton(renderer, achievementsButton);
            drawButton(renderer, exitButton);
        } else {
            // Calcul du FPS
            fps = (double) (clock() - startTime) / CLOCKS_PER_SEC;

            moveSpeed = 5. * fps;
            rotateSpeed = 225. * fps;

            fps = (int)(1. / fps);

            startTime = clock();


            // Gérer les mouvements du joueur
            keyboardInput(keystate);


            // Rendu de la scène
            renderScene();


            // Affichage du FPS
            if (showState) {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
                SDL_Rect block = {10, 10, SCREEN_WIDTH / 3 - 10, SCREEN_HEIGHT / 2 - 10};
                SDL_RenderFillRect(renderer, &block);

                char fpsText[25], text[25];
                if ((startTime - previousTime) * 1000 / CLOCKS_PER_SEC >= 500) {
                    previousTime = clock();
                    sprintf(fpsText, "FPS: %.0f", fps);
                }
                
                renderText(30, 30, 100, 25, fpsText, (SDL_Color){255, 255, 255, 255});

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect wallRect = {30, 70, SCREEN_WIDTH / 3 - 60, 2};
                SDL_RenderFillRect(renderer, &wallRect);

                sprintf(text, "Colision: %1d", colision);
                renderText(30, 80, 100, 25, text, (SDL_Color){255, 255, 255, 255});

                sprintf(text, "Texture: %1d", showTextures);
                renderText(30, 110, 92, 25, text, (SDL_Color){255, 255, 255, 255});

                sprintf(text, "Map: %1d", showMap);
                renderText(30, 140, 62, 25, text, (SDL_Color){255, 255, 255, 255});

                sprintf(text, "Rays map: %1d", showRays);
                renderText(30, 170, 100, 25, text, (SDL_Color){255, 255, 255, 255});
            }


            // Affichage de la map
            if(showMap) {
                for (int y = 0; y < mapHeight; y++) {
                    for (int x = 0; x < mapWidth; x++) {
                        switch (map[y][x]) {
                            case 1:
                                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                                break;
                            case 2:
                                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                                break;
                            case 3:
                                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                                break;
                            case 4:
                                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                                break;
                            case 5:
                                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                                break;
                            default:
                                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                                break;
                        }

                        SDL_Rect block = {SCREEN_WIDTH - (20 + x * mapWallSize), y * mapWallSize + 10, mapWallSize, mapWallSize};
                        SDL_RenderFillRect(renderer, &block);
                    }
                }

                int triangleSize = mapWallSize / 1.5;
                float angleRad = (player.angle) * M_PI / 180.0;
                int x1 = SCREEN_WIDTH - (20 + player.x * mapWallSize) + mapWallSize;
                int y1 = player.y * mapWallSize + 10;
                int x2 = x1 + cos(angleRad) * triangleSize;
                int y2 = y1 + sin(angleRad) * triangleSize;
                int x3 = x1 + cos(angleRad + 2.0 * M_PI / 3.0) * triangleSize;
                int y3 = y1 + sin(angleRad + 2.0 * M_PI / 3.0) * triangleSize;
                int x4 = x1 + cos(angleRad - 2.0 * M_PI / 3.0) * triangleSize;
                int y4 = y1 + sin(angleRad - 2.0 * M_PI / 3.0) * triangleSize;

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
                SDL_RenderDrawLine(renderer, x3, y3, x4, y4);
                SDL_RenderDrawLine(renderer, x4, y4, x1, y1);
            }
        }

        if (!cursorChanged) {
            SDL_SetCursor(defaultCursor);
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer);
    }


    // Fermeture de la fenêtre
    closeWindow();
    return 0;
}
