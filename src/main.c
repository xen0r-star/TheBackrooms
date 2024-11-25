#include "main.h"
#include "window.h"
#include "handle.h"
#include "render.h"
#include "data.h"
#include "menu.h"
#include "texture.h"
#include "utils.h"


GameState gameState = {
    .app = {800, 600, true, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0},
    .playerState = {{4.0, 4.0, 0.0}, 150.0, 3.0, 0.0, 0.0, 1, false, false, true, true},
    .menu = {MENU_MAIN},
    .mapState = {MAP_SIZE, MAP_SIZE, {{0}}, {{0}}, {0, 0, 0}},
    .graphics = {NULL, NULL, NULL},
    .settings = {0.2, 60.0, 1.0}
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
            calculateFPS(&state->app, &state->playerState);


            keyboardInput(state);
            if (state->app.controller != NULL) controllerInput(state);

            renderScene(state);
            itemFrame(state->app, state->playerState.selectFrame);

            if (state->playerState.showState) showStateInterface(&state->app, state->playerState);
            if (state->playerState.showMap)   showMapInterface(state->app, state->mapState, state->playerState);
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(state->app.renderer);
    }


    // Fermeture de la fenêtre
    closeWindow(state);
    return 0;
}
