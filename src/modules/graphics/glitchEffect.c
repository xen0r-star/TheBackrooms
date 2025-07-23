#include "graphics.h"

void glitchEffect(const AppState appState, GraphicsBuffers *graphicsBuffers, int speed)
{
    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    srand(clock() / (1000 / speed));

    // 1. Lignes de balayage (scan lines)
    for (int y = 0; y < screenHeight; y += 2)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            int index = y * screenWidth + x;
            graphicsBuffers->screenBuffers[index] = (50 << 24) | (0 << 16) | (0 << 8) | 0;
        }
    }

    // 2. Distorsions de ligne aléatoires
    for (int y = 0; y < screenHeight; y += 15)
    {                                  // Bandes de 15 pixels de hauteur
        int shift = (rand() % 10) - 5; // Décalage aléatoire entre -5 et +5 pixels

        for (int x = 0; x < screenWidth; x++)
        {
            int index = y * screenWidth + x;
            int shiftedX = x + shift;

            if (shiftedX >= 0 && shiftedX < screenWidth)
            {
                graphicsBuffers->screenBuffers[index] = graphicsBuffers->screenBuffers[y * screenWidth + shiftedX];
            }
        }
    }

    // 3. Bruit de couleur (artefacts)
    // for (int i = 0; i < screenWidth * screenHeight / 50; i++) { // Quelques pixels aléatoires
    //     int randomX = rand() % screenWidth;
    //     int randomY = rand() % screenHeight;
    //     int index = randomY * screenWidth + randomX;

    //     Uint8 r = rand() % 256;
    //     Uint8 g = rand() % 256;
    //     Uint8 b = rand() % 256;
    //     graphicsBuffers->screenBuffers[index] = (255 << 24) | (r << 16) | (g << 8) | b;
    // }

    // 4. Déformation de couleur
    // int bandHeight = 100;
    // int startY = rand() % (screenHeight - bandHeight);

    // for (int y = startY; y < startY + bandHeight; y += 2) {  // sauter une ligne sur 2
    //     for (int x = 0; x < screenWidth; x += 2) {            // sauter un pixel sur 2
    //         int index = y * screenWidth + x;
    //         Uint8 r = (graphicsBuffers->screenBuffers[index] >> 16) & 0xFF;
    //         Uint8 b = graphicsBuffers->screenBuffers[index] & 0xFF;

    //         int redX = x + 1;
    //         int blueX = x - 1;

    //         int randomValue = rand() % 18;

    //         if (randomValue == 3 && redX < screenWidth) {
    //             graphicsBuffers->screenBuffers[y * screenWidth + redX] = (255 << 24) | (r << 16) | (0 << 8) | 0;
    //         } else if (randomValue == 6 && blueX >= 0) {
    //             graphicsBuffers->screenBuffers[y * screenWidth + blueX] |= (255 << 24) | (0 << 16) | (0 << 8) | 70;
    //         }
    //     }
    // }
}