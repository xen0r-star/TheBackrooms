#include "utils.h"

int renderText(AppState *appState, int x, int y, int w, int h, const char *text, SDL_Color color) {
    SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(appState->font, text, color);
    if (!surfaceMessage) {
        printf("Erreur lors de la création de la surface texte: %s\n", TTF_GetError());
        return 1;
    }

    appState->message = SDL_CreateTextureFromSurface(appState->renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    if (!appState->message) {
        printf("Erreur lors de la création de la texture texte: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect message_rect;
    message_rect.x = x;  // Position en x
    message_rect.y = y;  // Position en y
    message_rect.w = w;  // Largeur du texte
    message_rect.h = h;   // Hauteur du texte

    SDL_RenderCopy(appState->renderer, appState->message, NULL, &message_rect);

    return 0;
}