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



void showStateInterface() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 190);
    SDL_Rect block = {10, 10, SCREEN_WIDTH / 3 - 10, SCREEN_HEIGHT / 2 - 10};
    SDL_RenderFillRect(renderer, &block);

    static char fpsText[25];
    char text[25];
    if ((startTime - previousTime) * 1000 / CLOCKS_PER_SEC >= 500) {
        previousTime = clock();
        printf("FPS: %.0f\n", fps);
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

void showMapInterface() {
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