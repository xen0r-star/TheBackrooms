#include "utils.h"
#include "render.h"

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}



int renderText(int x, int y, int w, int h, const char *text, SDL_Color color) {
    SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(font, text, color);
    if (!surfaceMessage) {
        printf("Erreur lors de la création de la surface texte: %s\n", TTF_GetError());
        return 1;
    }

    message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    if (!message) {
        printf("Erreur lors de la création de la texture texte: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect message_rect;
    message_rect.x = x;  // Position en x
    message_rect.y = y;  // Position en y
    message_rect.w = w;  // Largeur du texte
    message_rect.h = h;   // Hauteur du texte

    SDL_RenderCopy(renderer, message, NULL, &message_rect);

    return 0;
}


void drawButton(SDL_Renderer *renderer, Button button) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
    SDL_RenderFillRect(renderer, &button.rect);

    renderText(
        button.rect.x + (button.rect.w - (int) (strlen(button.text) * 10)) / 2, 
        button.rect.y + (button.rect.h - (int) (button.rect.h * .75)) / 2, 
        (int) (strlen(button.text) * 10), 
        (int) (button.rect.h * .75),
        button.text, (SDL_Color){0, 0, 0, 255}
    );
}

bool clickedButton(Button button, int mouseX, int mouseY) {
    return SDL_PointInRect(&(SDL_Point){mouseX, mouseY}, &button.rect);
}