#include "graphics/graphics.h"
#include "input/input.h"
#include "menu/menu.h"
#include "utils/utils.h"
#include "core/core.h"
#include "data/data.h"
#include "entities/entity.h"


GameState gameState = {
    .app = {800, 600, true, NULL, NULL, NULL, NULL, {NULL}, 0, 0, 0},
    .playerState = {{1.0, 1.0, 0.0}, 150.0, 3.0, 0.0, 0.0, 1, false, false, true, true},
    .entityState = {NULL, 0, NULL}, 
    .menu = {MENU_MAIN, BACKGROUND_MENU, 0},
    .mapState = {MAP_SIZE_LEVEL0, MAP_SIZE_LEVEL0, NULL, NULL, 0},
    .graphics = {NULL, NULL, NULL, {0}},
    .settings = {0.2, 85.0, 1.0}
};



int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;


    GameState *state = malloc(sizeof(GameState));
    *state = gameState;

    if (initializationWindow(state)  ) return 1;
    if (initializationMenu(state)    ) return 1;
    if (initializationScreen(state)  ) return 1;
    if (initializationTextures(state)) return 1;


    initializationEntity(&state->entityState, (Sprite){
        .x = 4.0,
        .y = 4.0,
        .scaleX = 0.8f,
        .scaleY = 0.8f,
        .texture_id = 4,
        .transform = {.transparency = 1.0f, .moveY = -64.0f}
    });


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

        if (hasWindowResize(&state->app)) {
            printf("REsize\n");
            SDL_DestroyTexture(state->graphics.screenBuffersTexture);
            free(state->graphics.screenBuffers);

            SDL_GetWindowSize(state->app.window, &state->app.screenWidth, &state->app.screenHeight);

            if (initializationScreen(state)) return 1;
            if (initializationMenu(state))   return 1;
        }

        // Nettoyage de l'écran
        SDL_SetRenderDrawColor(state->app.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state->app.renderer);
        
        if (state->menu.displayMenu != MENU_NONE) drawMenu(state);
        else {
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