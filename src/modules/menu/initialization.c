#include "menu.h"


int initializationMenu(GameState *state) {
    // Validate input parameters
    if (!state) {
        fprintf(stderr, "Error: Invalid game state for menu initialization\n");
        return -1;
    }
    
    int screenWidth = state->app.screenWidth;
    int screenHeight = state->app.screenHeight;
    
    // Validate screen dimensions
    if (screenWidth <= 0 || screenHeight <= 0) {
        fprintf(stderr, "Error: Invalid screen dimensions (%dx%d)\n", screenWidth, screenHeight);
        return -1;
    }
      
    // Initialize sound system only on first call
    static int soundsInitialized = 0;
    if (!soundsInitialized) {
        initSounds();
        playMenuMusic();
        soundsInitialized = 1;
    }

    // Define consistent button properties
    const int buttonWidth = 400;
    const int buttonHeight = 50;
    const int buttonSpacing = MENU_BUTTON_SPACING;
    const Color buttonColor = {137, 136, 113, 127};  // Backrooms aesthetic color
    const Color saveSlotColor = {0, 0, 0, 255};      // Black for save slots
    
    // Main menu buttons - vertically centered with consistent spacing
    state->menu.playButton = (Button){
        {screenWidth / 2, MENU_BUTTON_BASE_Y + 0 * buttonSpacing, buttonWidth, buttonHeight},
        buttonColor,
        "Play"  // TODO: Localize - was "Jouer"
    };

    state->menu.achievementsButton = (Button){
        {screenWidth / 2, MENU_BUTTON_BASE_Y + 1 * buttonSpacing, buttonWidth, buttonHeight},
        buttonColor,
        "Achievements"  // TODO: Localize - was "Succes"
    };

    state->menu.settingsButton = (Button){
        {screenWidth / 2, MENU_BUTTON_BASE_Y + 2 * buttonSpacing, buttonWidth, buttonHeight},
        buttonColor,
        "Settings"  // TODO: Localize - was "Parametres"
    };

    state->menu.exitButton = (Button){
        {screenWidth / 2, MENU_BUTTON_BASE_Y + 3 * buttonSpacing, buttonWidth, buttonHeight}, 
        buttonColor,
        "Exit"  // TODO: Localize - was "Quitter"
    };

    // Pause menu buttons - same layout as main menu
    state->menu.resumeGameButton = (Button){
        {screenWidth / 2, MENU_BUTTON_BASE_Y + 0 * buttonSpacing, buttonWidth, buttonHeight},
        buttonColor,
        "Resume"  // TODO: Localize - was "Reprendre"
    };

    state->menu.exitGameButton = (Button){
        {screenWidth / 2, MENU_BUTTON_BASE_Y + 3 * buttonSpacing, buttonWidth, buttonHeight}, 
        buttonColor,
        "Save and Exit"  // TODO: Localize - was "Quitter et sauvegarder"
    };

    // Save/Load menu configuration
    const int saveSlotHeight = 125;
    const int saveSlotSpacing = 145;
    const int saveSlotMargin = 60;  // Margin from screen edges
    
    // Save slot buttons - full width with proper margins
    state->menu.loadGame1 = (Button) {
        {screenWidth / 2, 100 + saveSlotSpacing * 0, screenWidth - saveSlotMargin * 2, saveSlotHeight}, 
        saveSlotColor,
        "Save Slot 1"  // Dynamic text based on save file existence
    };

    state->menu.loadGame2 = (Button) {
        {screenWidth / 2, 100 + saveSlotSpacing * 1, screenWidth - saveSlotMargin * 2, saveSlotHeight}, 
        saveSlotColor,
        "Save Slot 2"  // Dynamic text based on save file existence
    };

    state->menu.loadGame3 = (Button) {
        {screenWidth / 2, 100 + saveSlotSpacing * 2, screenWidth - saveSlotMargin * 2, saveSlotHeight}, 
        saveSlotColor,
        "Save Slot 3"  // Dynamic text based on save file existence
    };

    // Launch game button - positioned below save slots
    state->menu.launchGame = (Button) {
        {screenWidth / 2, 100 + saveSlotSpacing * 3, screenWidth - saveSlotMargin * 2, 100}, 
        saveSlotColor,
        "Start Game"
    };

    // Return button - positioned at bottom of screen
    const int returnButtonMargin = 45;  // Distance from bottom edge
    state->menu.returnButton = (Button) {
        {screenWidth / 2, screenHeight - returnButtonMargin, screenWidth - saveSlotMargin * 2, buttonHeight}, 
        saveSlotColor,
        "Back"  // TODO: Localize - was "Retourner"
    };

    // Initialize menu state
    state->menu.displayMenu = MENU_MAIN;
    state->menu.backgroundType = BACKGROUND_MENU;
    state->menu.scrollOffset = 0;
    
    printf("Menu system initialized with %dx%d screen dimensions\n", screenWidth, screenHeight);
    return 0;
}