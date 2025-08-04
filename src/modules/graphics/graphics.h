/**
 * @file graphics.h
 * @brief Graphics rendering system for The Backrooms
 * 
 * This module handles all graphics operations including window management,
 * texture loading, screen buffer management, and special effects like
 * glitch effects for the horror atmosphere.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../../main.h"
#include "../core/core.h"
#include "../entities/entity.h"

// Error codes for graphics operations
#define GRAPHICS_SUCCESS           0
#define GRAPHICS_ERROR_INIT       -1
#define GRAPHICS_ERROR_MEMORY     -2
#define GRAPHICS_ERROR_RESOURCE   -3

/**
 * @brief Main scene rendering function
 * 
 * Renders the complete 3D scene using raycasting algorithm.
 * Includes floor/ceiling rendering, wall rendering with textures,
 * and sprite entities with proper depth sorting.
 * 
 * @param state Pointer to the main game state
 */
extern void renderScene(GameState *state);

/**
 * @brief Applies CRT-style glitch effects to the screen
 * 
 * Creates visual distortions including scan lines, horizontal shifts,
 * and color artifacts to enhance the horror atmosphere.
 * 
 * @param appState Application state containing screen dimensions
 * @param graphicsBuffers Graphics buffers to apply effects to
 * @param speed Effect animation speed (higher = faster changes)
 */
extern void glitchEffect(const AppState appState, GraphicsBuffers *graphicsBuffers, int speed);

/**
 * @brief Initializes texture system and loads all game textures
 * @param state Game state to initialize textures for
 * @return GRAPHICS_SUCCESS on success, error code on failure
 */
extern int initializationTextures(GameState *state);

/**
 * @brief Initializes screen buffers and rendering textures
 * @param state Game state to initialize screen for
 * @return GRAPHICS_SUCCESS on success, error code on failure
 */
extern int initializationScreen(GameState *state);

/**
 * @brief Creates SDL window and initializes graphics context
 * @param state Game state to initialize window for
 * @return GRAPHICS_SUCCESS on success, error code on failure
 */
extern int initializationWindow(GameState *state);

/**
 * @brief Properly closes window and cleans up graphics resources
 * @param state Game state to cleanup
 * @return GRAPHICS_SUCCESS on success, error code on failure
 */
extern int closeWindow(GameState *state);

/**
 * @brief Checks if window has been resized
 * @param appState Application state to check
 * @return true if window was resized, false otherwise
 */
extern bool hasWindowResize(AppState *appState);

/**
 * @brief Calculates field of view for rendering
 * @param fov Field of view in degrees
 * @return Calculated FOV value for rendering pipeline
 */
extern float calculateFovRender(float fov);

/**
 * @brief Calculates direction vectors from angle
 * @param angle Rotation angle in degrees
 * @param dirX Pointer to store X direction component
 * @param dirY Pointer to store Y direction component
 */
extern void calculateDirRender(float angle, float *dirX, float *dirY);

#endif /* GRAPHICS_H */