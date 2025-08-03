/**
 * @file utils.h
 * @brief Utility functions for The Backrooms game
 * 
 * This module provides essential utility functions for text rendering,
 * FPS calculation, debug interfaces, map visualization, and UI elements.
 * Supports various text alignments, performance monitoring, and game state display.
 */

#ifndef UTILS_H
#define UTILS_H

#include "../../main.h"
#include "../map/map.h"
#include "../graphics/graphics.h"

// UI and rendering constants
#define FPS_UPDATE_INTERVAL_MS    500    // FPS display update frequency
#define DEBUG_INTERFACE_OPACITY   190    // Semi-transparent debug overlay
#define MAP_WINDOW_SCALE         0.4f    // Map display scale factor

// Inventory interface constants
#define INVENTORY_MAX_SLOTS       7      // Maximum number of inventory slots
#define INVENTORY_BAR_WIDTH       358    // Inventory bar width in pixels
#define INVENTORY_BAR_HEIGHT      64     // Inventory bar height in pixels
#define INVENTORY_FRAME_SIZE      64     // Selection frame size in pixels
#define INVENTORY_SLOT_SPACING    49     // Spacing between inventory slots
#define INVENTORY_MARGIN_BOTTOM   20     // Distance from bottom screen edge

// Minimap interface constants
#define MINIMAP_SCREEN_RATIO     0.4f    // Minimap size as ratio of screen
#define MINIMAP_MAX_DEFORMATION  1.5f    // Maximum perspective deformation
#define MINIMAP_MARGIN_BOTTOM    25      // Distance from bottom screen edge
#define MINIMAP_PLAYER_SIZE      4       // Player indicator size in pixels

// Minimap colors (RGBA)
#define MINIMAP_BG_COLOR         (Color){220, 180, 120, 255}  // Background
#define MINIMAP_WALL1_COLOR      (Color){ 54,  43,  26, 255}  // Wall type 1
#define MINIMAP_WALL2_COLOR      (Color){ 92,  70,  37, 255}  // Wall type 2
#define MINIMAP_WALL3_COLOR      (Color){ 92,  70,  37, 255}  // Wall type 3
#define MINIMAP_WALL4_COLOR      (Color){ 92,  70,  37, 255}  // Wall type 4
#define MINIMAP_WALL5_COLOR      (Color){ 92,  70,  37, 255}  // Wall type 5
#define MINIMAP_FLOOR_COLOR      (Color){160, 125,  72, 255}  // Floor/empty
#define MINIMAP_PLAYER_COLOR     (Color){196,  65,  65, 255}  // Player indicator

/**
 * @brief Calculates frame rate and updates movement speed
 * 
 * Computes the current frame rate using high-resolution timing and
 * updates player movement speeds based on frame time for smooth,
 * frame-rate-independent movement.
 * 
 * @param appState Application state containing timing information
 * @param playerState Player state to update with frame-dependent speeds
 * 
 * @note Movement speeds are automatically scaled by frame time
 * @note FPS calculation uses clock() for cross-platform compatibility
 */
extern void calculateFPS(AppState *appState, PlayerState *playerState);

/**
 * @brief Renders text with specified alignment and styling
 * 
 * Renders UTF-8 text to the screen with configurable alignment, position,
 * color, and font size. Supports left, center, and right alignment with
 * automatic positioning calculations.
 * 
 * @param appState Application state containing renderer and font resources
 * @param alignment Text alignment (LEFT, CENTER, RIGHT)
 * @param x Base X coordinate for text positioning
 * @param y Y coordinate for text baseline
 * @param text UTF-8 encoded text string to render
 * @param color Text color with RGBA components
 * @param type Font size type (TEXT_XS, TEXT_S, TEXT_L, TEXT_TITLE)
 * @return 0 on success, non-zero on error
 * 
 * @note Handles automatic memory management for SDL surfaces and textures
 * @note Supports multi-byte UTF-8 character encoding
 * @note X coordinate is adjusted based on alignment and text width
 */
extern int renderText(AppState *appState, Alignment alignment, int x, int y, 
                     const char *text, Color color, TextType type);

/**
 * @brief Displays debug information overlay
 * 
 * Renders a semi-transparent debug interface showing FPS, collision state,
 * texture rendering state, and map display state. Updates periodically
 * to avoid performance impact from constant text rendering.
 * 
 * @param appState Application state containing rendering and timing data
 * @param playerState Player state containing debug flag values
 * @param entityState Entity state for sprite count and depth buffer
 * 
 * @note Interface appears in top-left corner with semi-transparent background
 * @note FPS display updates every 500ms to reduce performance overhead
 * @note Shows boolean states as 0/1 for compactness
 */
extern void showStateInterface(AppState *appState, const PlayerState playerState, const EntityState entityState);

/**
 * @brief Renders minimap with explored areas and player position
 * 
 * Displays a perspective-corrected minimap showing discovered areas,
 * walls, and the player's current position. Uses a window-based view
 * centered on the player with perspective deformation effect.
 * 
 * @param appState Application state containing screen dimensions and renderer
 * @param mapState Map state with tile data and discovery information
 * @param playerState Player state for position and centering
 * 
 * @note Only shows areas that have been discovered by the player
 * @note Uses perspective deformation for 3D-like appearance
 * @note Player position is highlighted with a red square
 * @note Map colors represent different wall and floor types
 */
extern void showMapInterface(const AppState appState, const MapState mapState, 
                           const PlayerState playerState);

/**
 * @brief Renders inventory bar with item selection highlight
 * 
 * Displays the game's inventory interface with item slots and selection
 * highlighting. Loads and renders inventory bar and selection frame textures
 * with proper positioning and scaling.
 * 
 * @param appState Application state containing renderer and screen dimensions
 * @param selectFrame Currently selected inventory slot (1-based index)
 * 
 * @note Inventory bar is centered horizontally at bottom of screen
 * @note Selection frame highlights the currently active slot
 * @note Textures are loaded and destroyed each frame (needs optimization)
 * @note Uses hardcoded texture paths relative to executable
 */
extern void itemFrame(const AppState appState, int selectFrame);

#endif /* UTILS_H */