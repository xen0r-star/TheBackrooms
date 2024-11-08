#include "main.h"
#include "initialization.h"
#include "handle.h"
#include "render.h"
#include "data.h"
#include "menu.h"
#include "texture.h"
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

    // Chargement textures
    initializationTextures();


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


        if (displayMenu != 0) menu();
        else {
            SDL_SetRelativeMouseMode(SDL_TRUE);

            // Calcul du FPS
            calculateFPS();


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
