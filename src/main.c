#include "main.h"
#include "window.h"
#include "handle.h"
#include "render.h"
#include "data.h"
#include "menu.h"
#include "texture.h"
#include "utils.h"


int screenWidth = 800;
int screenHeight = 600;

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
Button loadGame1;
Button loadGame2;
Button loadGame3;
Button launchGame;

bool running = true;
bool showMap = false;
bool showState = false;
bool showTextures = true;
bool colision = true;
int displayMenu = MENU_MAIN;

Player player = {4.0, 4.0, 0.0};
float rotateSpeed = 150.0;
float moveSpeed = 3.0;
float sensitivity = 0.2;
int selectFrame = 1;

float playerRotateSpeed, playerMoveSpeed;

int mapWidth = MAP_SIZE;
int mapHeight = MAP_SIZE;
int map[MAP_SIZE][MAP_SIZE];
int mapDiscovered[MAP_SIZE][MAP_SIZE];

int typeLaunchGame[3] = {0, 0, 0};

float fps;
clock_t startTime, previousTime;



int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;


    if (initializationWindow()  ) return 1;
    if (initializationMenu()    ) return 1;
    if (initializationTextures()) return 1;


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
        SDL_GetWindowSize(window, &screenWidth, &screenHeight);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        printf("screen width: %d, screen height: %d\n", screenWidth, screenHeight);


        if (displayMenu != MENU_NONE) drawMenu();
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
    closeWindow();
    return 0;
}
