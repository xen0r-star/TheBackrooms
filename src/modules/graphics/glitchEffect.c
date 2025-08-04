#include "graphics.h"
#include <time.h>


static int validateGlitchParams(const AppState appState, const GraphicsBuffers *graphicsBuffers, int speed) {
    if (!graphicsBuffers || !graphicsBuffers->screenBuffers) {
        return -1;
    }
    
    if (appState.screenWidth <= 0 || appState.screenHeight <= 0) {
        return -1;
    }
    
    if (speed <= 0 || speed > 100) {
        return -1;
    }
    
    return 0;
}

static void applyScanLines(Uint32 *screenBuffers, int screenWidth, int screenHeight) {
    // Apply scan lines every other row for CRT effect
    for (int y = 0; y < screenHeight; y += 2) {
        for (int x = 0; x < screenWidth; x++) {
            int index = y * screenWidth + x;
            // Darken scan lines with semi-transparent black
            screenBuffers[index] = (50 << 24) | (0 << 16) | (0 << 8) | 0;
        }
    }
}

static void applyHorizontalDistortion(Uint32 *screenBuffers, int screenWidth, int screenHeight) {
    // Random horizontal line distortions
    for (int y = 0; y < screenHeight; y += 15) { // Bands of 15 pixels height
        int shift = (rand() % 10) - 5; // Random shift between -5 and +5 pixels

        for (int x = 0; x < screenWidth; x++) {
            int index = y * screenWidth + x;
            int shiftedX = x + shift;

            // Bounds checking to prevent buffer overflow
            if (shiftedX >= 0 && shiftedX < screenWidth) {
                screenBuffers[index] = screenBuffers[y * screenWidth + shiftedX];
            }
        }
    }
}

__attribute__((unused))
static void applyColorNoise(Uint32 *screenBuffers, int screenWidth, int screenHeight) {
    // Suppress unused parameter warnings for placeholder function
    (void)screenBuffers;
    (void)screenWidth;
    (void)screenHeight;
    
    // Add random color noise (currently disabled for performance)
    // Uncomment for additional visual noise effect
    /*
    for (int i = 0; i < screenWidth * screenHeight / 50; i++) { // Some random pixels
        int randomX = rand() % screenWidth;
        int randomY = rand() % screenHeight;
        int index = randomY * screenWidth + randomX;

        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        screenBuffers[index] = (255 << 24) | (r << 16) | (g << 8) | b;
    }
    */
}

__attribute__((unused))
static void applyChromaticAberration(Uint32 *screenBuffers, int screenWidth, int screenHeight) {
    // Suppress unused parameter warnings for placeholder function
    (void)screenBuffers;
    (void)screenWidth;
    (void)screenHeight;
    
    // Color channel separation effect (currently disabled)
    // Uncomment for chromatic aberration effect
    /*
    int bandHeight = 100;
    int startY = rand() % (screenHeight - bandHeight);

    for (int y = startY; y < startY + bandHeight; y += 2) {  // Skip every other line
        for (int x = 0; x < screenWidth; x += 2) {            // Skip every other pixel
            int index = y * screenWidth + x;
            Uint8 r = (screenBuffers[index] >> 16) & 0xFF;
            Uint8 b = screenBuffers[index] & 0xFF;

            int redX = x + 1;
            int blueX = x - 1;

            int randomValue = rand() % 18;

            if (randomValue == 3 && redX < screenWidth) {
                screenBuffers[y * screenWidth + redX] = (255 << 24) | (r << 16) | (0 << 8) | 0;
            } else if (randomValue == 6 && blueX >= 0) {
                screenBuffers[y * screenWidth + blueX] |= (255 << 24) | (0 << 16) | (0 << 8) | 70;
            }
        }
    }
    */
}


void glitchEffect(const AppState appState, GraphicsBuffers *graphicsBuffers, int speed) {
    // Validate input parameters
    if (validateGlitchParams(appState, graphicsBuffers, speed) != 0) {
        return;
    }

    int screenWidth = appState.screenWidth;
    int screenHeight = appState.screenHeight;

    // Seed random number generator based on time and speed
    // This creates time-varying but speed-controlled randomness
    srand((unsigned int)(clock() / (1000 / speed)));

    // Apply various glitch effects in sequence
    applyScanLines(graphicsBuffers->screenBuffers, screenWidth, screenHeight);
    applyHorizontalDistortion(graphicsBuffers->screenBuffers, screenWidth, screenHeight);
    
    // Additional effects (currently disabled for performance/aesthetics)
    // applyColorNoise(graphicsBuffers->screenBuffers, screenWidth, screenHeight);
    // applyChromaticAberration(graphicsBuffers->screenBuffers, screenWidth, screenHeight);
}