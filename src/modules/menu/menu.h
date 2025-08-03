/**
 * @file menu.h
 * @brief Menu system for The Backrooms game interface
 * 
 * This module provides a comprehensive menu system including main menu,
 * settings, achievements, save/load functionality, and pause menu.
 * Handles button interactions, background rendering, and state management.
 */

#ifndef MENU_H
#define MENU_H

#include "../../main.h"
#include "../map/map.h"
#include "../data/data.h"
#include "../utils/utils.h"

// Menu system constants
#define MAX_SAVE_SLOTS           3
#define MENU_BUTTON_SPACING     70    // Vertical spacing between menu buttons
#define MENU_BUTTON_BASE_Y     300    // Starting Y position for main menu buttons
#define MENU_SCROLL_SPEED       20    // Pixels per scroll event

/**
 * @brief Initializes menu system and creates all buttons
 * 
 * Sets up all menu buttons with proper positioning based on screen dimensions.
 * Configures button properties including size, color, and text content.
 * 
 * @param state Game state containing screen dimensions and menu data
 * @return 0 on success, non-zero on failure
 * 
 * @note Must be called after graphics initialization
 * @note Button positions are calculated relative to screen size
 */
extern int initializationMenu(GameState *state);

/**
 * @brief Checks if a point is within a button's clickable area
 * 
 * Performs point-in-rectangle collision detection for button interaction.
 * Accounts for button centering and proper hit box calculation.
 * 
 * @param button Button to test against
 * @param mouseX Mouse X coordinate
 * @param mouseY Mouse Y coordinate
 * @return true if point is within button bounds, false otherwise
 * 
 * @note Button rectangles are centered on their position coordinates
 */
extern bool clickedButton(Button button, int mouseX, int mouseY);

/**
 * @brief Renders the appropriate menu screen
 * 
 * Main menu rendering function that handles all menu states and
 * displays appropriate interface elements based on current menu state.
 * 
 * @param state Game state containing menu and rendering information
 * 
 * @note Automatically handles mouse cursor state based on hover detection
 * @note Updates save slot information dynamically
 */
extern void drawMenu(GameState *state);

/**
 * @brief Renders multiple buttons with hover detection
 * 
 * Variadic function that renders a collection of buttons and handles
 * hover state detection. Updates cursor appearance based on interaction.
 * 
 * @param appState Application state for rendering
 * @param mouseX Current mouse X coordinate
 * @param mouseY Current mouse Y coordinate
 * @param buttonCount Number of buttons to process
 * @param ... Variable arguments list of Button structures
 * 
 * @note Uses variadic arguments for flexible button count
 * @note Automatically manages cursor state changes
 */
extern void handleButtons(AppState *appState, int mouseX, int mouseY, int buttonCount, ...);

/**
 * @brief Processes menu button clicks and state transitions
 * 
 * Handles all menu navigation logic based on button clicks.
 * Manages state transitions between different menu screens.
 * 
 * @param state Game state to modify based on user interaction
 * @param mouseX Mouse X coordinate for click detection
 * @param mouseY Mouse Y coordinate for click detection
 * 
 * @note Handles save data loading and game state initialization
 * @note Manages mouse capture mode for game/menu transitions
 */
extern void handleMenuButtons(GameState *state, int mouseX, int mouseY);

/**
 * @brief Renders a single button with specified visual state
 * 
 * Draws a button with appropriate visual feedback based on interaction state.
 * Handles normal, hover, and selected button appearances.
 * 
 * @param appState Application state for rendering context
 * @param button Button structure containing position and appearance data
 * @param type Button visual state (normal, hover, selected)
 * 
 * @note Button text is automatically centered
 * @note Visual feedback includes color changes and selection borders
 */
extern void drawButton(AppState *appState, Button button, ButtonType type);

/**
 * @brief Renders background based on menu context
 * 
 * Displays appropriate background for current menu state, either
 * animated 3D scene for main menu or dimmed game view for pause menu.
 * 
 * @param state Game state containing rendering and menu information
 * @param backgroundType Type of background to render
 * 
 * @note Menu background includes animated rotating camera
 * @note Game background shows dimmed current game state
 */
extern void background(GameState *state, BackgroundType backgroundType);

/**
 * @brief Shows loading screen with progress indication
 * 
 * Displays a loading screen overlay with animated progress bar and status message.
 * Used during world generation, save file operations, and asset loading.
 * 
 * @param state Game state containing rendering context
 * @param message Loading status message to display
 * @param progress Loading progress from 0.0 to 1.0
 * 
 * @note Call this function in your loading loop with updated progress
 * @note Remember to call SDL_RenderPresent() after this function
 * @note Progress bar includes smooth color transitions and spinning indicator
 */
extern void showLoadingScreen(GameState *state, const char *message, float progress);

#endif /* MENU_H */