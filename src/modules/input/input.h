/**
 * @file input.h
 * @brief Input handling system for The Backrooms
 * 
 * This module provides comprehensive input handling for keyboard, mouse,
 * and game controller devices. Supports both event-driven and continuous
 * input processing for smooth gameplay.
 */

#ifndef INPUT_H
#define INPUT_H

#include "../../main.h"
#include "../menu/menu.h"
#include "../graphics/graphics.h"

// Input configuration constants
#define ANALOG_DEADZONE 8000        // Controller analog stick deadzone
#define ROTATION_SENSITIVITY 0.5f   // Base rotation sensitivity
#define MOVEMENT_DAMPING 0.5f       // Strafe movement speed multiplier

// Movement directions for clarity
typedef enum {
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_STRAFE_LEFT,
    MOVE_STRAFE_RIGHT
} MovementDirection;

/**
 * @brief Handles keyboard key press events
 * 
 * Processes single-press keyboard events like menu toggles,
 * debug mode switches, and system commands.
 * 
 * @param state Game state to modify
 * @param event SDL event containing key information
 */
extern void keyboardDown(GameState *state, const SDL_Event event);

/**
 * @brief Handles game controller button press events
 * 
 * Processes controller button presses for menu navigation,
 * debug toggles, and inventory management.
 * 
 * @param state Game state to modify
 * @param event SDL event containing controller button information
 */
extern void controllerDown(GameState *state, const SDL_Event event);

/**
 * @brief Handles mouse input events
 * 
 * Processes mouse movement for camera rotation, scroll wheel
 * for inventory/menu navigation, and button clicks for UI interaction.
 * 
 * @param state Game state to modify
 * @param event SDL event containing mouse information
 */
extern void mouseHandle(GameState *state, const SDL_Event event);

/**
 * @brief Processes continuous keyboard input
 * 
 * Handles held key states for smooth player movement and rotation.
 * Called every frame to ensure responsive controls.
 * 
 * @param state Game state to update with movement
 */
extern void keyboardInput(GameState *state);

/**
 * @brief Processes continuous controller input
 * 
 * Handles analog stick input for smooth movement and camera rotation.
 * Called every frame for responsive controller support.
 * 
 * @param state Game state to update with movement
 */
extern void controllerInput(GameState *state);

#endif /* INPUT_H */