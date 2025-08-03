#include "utils.h"


int renderText(AppState *appState, Alignment alignment, int x, int y, 
               const char *text, Color color, TextType type) {
    // Validate input parameters
    if (!appState || !appState->renderer || !text) {
        fprintf(stderr, "Error: Invalid parameters for text rendering\n");
        return 1;
    }
    
    // Validate text type bounds
    if (type < 0 || type >= TEXT_TYPE_COUNT) {
        fprintf(stderr, "Error: Invalid text type %d\n", type);
        return 1;
    }
    
    TTF_Font *font = appState->text[type];
    if (!font) {
        fprintf(stderr, "Error: Font not loaded for text type %d\n", type);
        return 1;
    }
    
    // Skip rendering empty strings
    if (strlen(text) == 0) {
        return 0;
    }

    // Create text surface with anti-aliasing
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface *surfaceText = TTF_RenderUTF8_Blended(font, text, sdlColor);
    
    if (!surfaceText) {
        fprintf(stderr, "Error creating text surface: %s\n", TTF_GetError());
        return 1;
    }

    // Measure text dimensions for alignment calculations
    int textWidth, textHeight;
    if (TTF_SizeUTF8(font, text, &textWidth, &textHeight) != 0) {
        fprintf(stderr, "Error measuring text dimensions: %s\n", TTF_GetError());
        SDL_FreeSurface(surfaceText);
        return 1;
    }

    // Create texture from surface
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(appState->renderer, surfaceText);
    SDL_FreeSurface(surfaceText);  // Free surface immediately after texture creation
    
    if (!textureText) {
        fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
        return 1;
    }

    // Calculate final position based on alignment
    int finalX = x;
    switch (alignment) {
        case CENTER:
            finalX = x - (textWidth / 2);
            break;
        case RIGHT:
            finalX = x - textWidth;
            break;
        case LEFT:
        default:
            finalX = x;
            break;
    }

    // Create destination rectangle (vertically centered on Y coordinate)
    SDL_Rect destRect = {
        finalX,
        y - (textHeight / 2),
        textWidth,
        textHeight
    };

    // Render the text texture
    int result = SDL_RenderCopy(appState->renderer, textureText, NULL, &destRect);
    if (result != 0) {
        fprintf(stderr, "Error rendering text: %s\n", SDL_GetError());
    }

    // Clean up texture
    SDL_DestroyTexture(textureText);

    return (result == 0) ? 0 : 1;
}