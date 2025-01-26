#include "utils.h"

int renderText(AppState *appState, Alignment alignment, int x, int y, const char *text, Color color, TextType type) {
    TTF_Font *font = appState->text[type];
    
    SDL_Surface *surfaceText = TTF_RenderUTF8_Blended(font, text, (SDL_Color){color.r, color.g, color.b, color.a});

    if (!surfaceText) {
        printf("Erreur lors de la création de la surface texte: %s\n", TTF_GetError());
        return 1;
    }

    int textWidth, textHeight;
    if (TTF_SizeText(font, text, &textWidth, &textHeight) != 0) {
        printf("Erreur mesure texte: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return 1;
    }

    SDL_Texture *textureText = SDL_CreateTextureFromSurface(appState->renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    if (!textureText) {
        printf("Erreur lors de la création de la texture texte: %s\n", SDL_GetError());
        return 1;
    }

    if (alignment == CENTER) {
        x -= textWidth / 2;
    } else if (alignment == RIGHT) {
        x -= textWidth;
    }

    SDL_Rect text_rect = {
        x,
        y - (textHeight / 2),
        textWidth,
        textHeight
    };

    SDL_RenderCopy(appState->renderer, textureText, NULL, &text_rect);

    SDL_DestroyTexture(textureText);

    return 0;
}