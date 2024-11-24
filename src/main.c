#include "main.h"
#include "window.h"
#include "handle.h"
#include "render.h"
#include "data.h"
#include "menu.h"
#include "texture.h"
#include "utils.h"


// int screenWidth = 800;
// int screenHeight = 600;

// SDL_Renderer *renderer = NULL;
// SDL_Window *window = NULL;
// const Uint8 *keystate;
// SDL_GameController *controller = NULL;
// TTF_Font *font = NULL;
// SDL_Texture *message = NULL;
// Uint32 *screenBuffers;
// SDL_Texture* screenBuffersTexture;
// Uint32 **textureBuffers;
// SDL_Texture *backgroundTexture = NULL;

// Button playButton;
// Button achievementsButton;
// Button settingsButton;
// Button exitButton;
// Button resumeGameButton;
// Button extiGameButton;
// Button loadGame1;
// Button loadGame2;
// Button loadGame3;
// Button launchGame;

// bool running = true;
// bool showMap = false;
// bool showState = false;
// bool showTextures = true;
// bool colision = true;
// int displayMenu = MENU_MAIN;

// Player player = {4.0, 4.0, 0.0};
// float rotateSpeed = 150.0;
// float moveSpeed = 3.0;
// float sensitivity = 0.2;
// int selectFrame = 1;

// float playerRotateSpeed, playerMoveSpeed;

// int mapWidth = MAP_SIZE;
// int mapHeight = MAP_SIZE;
// int map[MAP_SIZE][MAP_SIZE];
// int mapDiscovered[MAP_SIZE][MAP_SIZE];

// int typeLaunchGame[3] = ;

// float fps;
// clock_t startTime, previousTime;


GameState gameState = {
    .app = {800, 600, true, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0},
    .playerState = {{4.0, 4.0, 0.0}, 150.0, 3.0, 0.2, 0.0, 0.0, 1, false, false, true, true},
    .menu = {MENU_MAIN},
    .mapState = {MAP_SIZE, MAP_SIZE, {{0}}, {{0}}, {0, 0, 0}},
    .graphics = {NULL, NULL, NULL}
};



int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;


    GameState *state = malloc(sizeof(GameState));
    *state = gameState;


    if (initializationWindow(state)  ) return 1;
    if (initializationMenu(state)    ) return 1;
    if (initializationTextures(state)) return 1;


    // Boucle principale
    SDL_Event event;
    state->app.startTime = clock(), state->app.previousTime = clock();
    while (state->app.running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                state->app.running = false;
            }

            mouseHandle(state, event);
            keyboardDown(state, event);

            if (state->app.controller != NULL) controllerDown(state, event);
        }


        // Nettoyage de l'écran
        SDL_GetWindowSize(state->app.window, &state->app.screenWidth, &state->app.screenHeight);
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state->app.renderer);

        if (state->menu.displayMenu != MENU_NONE) drawMenu(state);
        else {
            SDL_SetRelativeMouseMode(SDL_TRUE);

            // Calcul du FPS
            calculateFPS(state);


            keyboardInput(state);
            if (state->app.controller != NULL) controllerInput(state);

            renderScene(state);
            itemFrame(state, state->playerState.selectFrame);

            if (state->playerState.showState) showStateInterface(state);
            if (state->playerState.showMap)   showMapInterface(state);
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(state->app.renderer);
    }


    // Fermeture de la fenêtre
    closeWindow(state);
    return 0;
}
