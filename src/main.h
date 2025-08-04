/**
 * @file main.h
 * @brief Core header file for The Backrooms game
 * 
 * Contains all essential data structures, enumerations, constants, and type definitions
 * used throughout the game. Serves as the central header included by all modules.
 * 
 * This file defines the complete game architecture including:
 * - Application state management
 * - Player and entity systems
 * - Graphics and rendering pipeline
 * - Menu and UI structures
 * - Map and world data
 * - Settings and configuration
 */

#ifndef MAIN_H
#define MAIN_H

// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

// SDL2 library includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Game configuration constants
#define WINDOW_TITLE "ESCAPE THE BACKROOMS"    // Main window title
#define TEXTURE_SIZE 64                         // Standard texture resolution (64x64)
#define NUMBER_TEXTURES 5                       // Total number of wall/floor textures
#define MAP_SIZE_LEVEL0 6                       // Grid size for Level 0 rooms

// Asset file paths (relative to executable)
#define FONT_ROBOTO "font/Roboto-Regular.ttf"   // Primary UI font
#define FONT_JERSEY "font/Jersey25-Regular.ttf" // Display/title font
#define ICON_FILE "image/The_Backrooms_logo.png" // Application icon

// Mathematical constant for PI
#define PI 3.14159265358979323846


/**
 * @brief Text rendering size types
 * 
 * Defines standardized font sizes for consistent UI typography.
 * Sizes range from extra-small (26px) to large titles (94px).
 */
typedef enum {
    TEXT_XS,        // 26px - Small UI text, debug info
    TEXT_S,         // 28px - Regular UI text, labels
    TEXT_M,         // 32px - Standard body text
    TEXT_L,         // 36px - Emphasis text, headings
    TEXT_XL,        // 48px - Large headings
    TEXT_XXL,       // 72px - Major headings
    TEXT_TITLE,     // 94px - Game title, splash screen
    TEXT_TYPE_COUNT // Count of text types for array sizing
} TextType;

/**
 * @brief Text alignment options for rendering
 * 
 * Controls horizontal text positioning relative to specified coordinates.
 */
typedef enum {
    LEFT,    // Align text to the left of X coordinate
    CENTER,  // Center text on X coordinate
    RIGHT    // Align text to the right of X coordinate
} Alignment;

/**
 * @brief Button visual states for UI interactions
 * 
 * Defines visual feedback states for interactive UI elements.
 */
typedef enum {
    BUTTON_NORMAL,   // Default button appearance
    BUTTON_SELECTED, // Button is currently selected/focused
    BUTTON_HOVER     // Mouse cursor is hovering over button
} ButtonType;

/**
 * @brief Game menu state enumeration
 * 
 * Controls which menu screen is currently displayed.
 * MENU_NONE indicates in-game state (no menu overlay).
 */
typedef enum {
    MENU_NONE,         // No menu (in-game)
    MENU_MAIN,         // Main menu (start screen)
    MENU_LOAD,         // Save game selection
    MENU_ACHIEVEMENTS, // Achievement display
    MENU_SETTINGS,     // Game settings/options
    MENU_BREAK         // Pause menu (in-game)
} MenuType;

/**
 * @brief Background rendering types
 * 
 * Determines which background style to render behind menus and UI.
 */
typedef enum {
    BACKGROUND_MENU,        // Static menu background
    BACKGROUND_GAME,        // Blurred/dimmed game background
    BACKGROUND_TYPE_COUNT   // Count for array sizing
} BackgroundType;

/**
 * @brief 2D player position and orientation
 * 
 * Stores the player's world position and facing direction.
 * Position uses floating-point for smooth movement between grid cells.
 */
typedef struct {
    float x, y;    // World position coordinates
    float angle;   // Facing direction in radians
} Player;

/**
 * @brief Sprite transformation effects
 * 
 * Additional visual effects applied to sprites during rendering.
 * Used for animations, transparency, and vertical positioning.
 */
typedef struct {
    float transparency; // Alpha transparency (0.0 = invisible, 1.0 = opaque)
    float moveY;        // Vertical offset in pixels (for bobbing, floating effects)
} transform_Sprite;
    
/**
 * @brief 3D sprite entity definition
 * 
 * Represents a 3D sprite object in the game world with position,
 * scaling, texture assignment, and visual effects.
 */
typedef struct {
    double x, y;              // World position (double precision for accuracy)
    float scaleX, scaleY;     // Horizontal and vertical scaling factors
    int texture_id;           // Index into texture array
    transform_Sprite transform; // Additional visual effects
} Sprite;

/**
 * @brief RGBA color definition
 * 
 * Standard 32-bit color with red, green, blue, and alpha channels.
 * Each component ranges from 0-255.
 */
typedef struct{
    int r, g, b, a; // Red, Green, Blue, Alpha (0-255 each)
} Color;

/**
 * @brief Rectangle definition for UI layout
 * 
 * Standard rectangle with position and dimensions.
 * Used for button bounds, rendering areas, and collision detection.
 */
typedef struct {
    int x, y;  // Top-left corner position
    int w, h;  // Width and height
} Rect;

/**
 * @brief UI button definition
 * 
 * Complete button definition including visual appearance and text content.
 * Used throughout the menu system for interactive elements.
 */
typedef struct {
    Rect rect;   // Button boundaries and position
    Color color; // Button background/border color
    char *text;  // Button label text
} Button;

/**
 * @brief Font specification
 * 
 * Defines font family and size for text rendering operations.
 * Used by the text rendering system for consistent typography.
 */
typedef struct {
    char *family; // Font file path
    int size;     // Font size in pixels
} Font;

/**
 * @brief Raycasting optimization cache
 * 
 * Pre-calculated values for the raycasting rendering pipeline.
 * Reduces redundant trigonometric calculations during frame rendering.
 */
typedef struct {
    float dirX, dirY;    // Player direction vector (normalized)
    float fovRender;     // Camera plane vector for field of view
} RenderCache;

/**
 * @brief Core application state
 * 
 * Contains all SDL resources, window management, input state,
 * and fundamental application data needed across all modules.
 */
typedef struct {
    // Window and rendering
    int screenWidth;           // Current window width in pixels
    int screenHeight;          // Current window height in pixels
    bool running;              // Main loop continuation flag

    // SDL resources
    SDL_Renderer       *renderer;    // Main graphics renderer
    SDL_Window         *window;      // Application window
    const Uint8        *keystate;    // Current keyboard state
    SDL_GameController *controller;  // Connected game controller (if any)
    TTF_Font           *text[TEXT_TYPE_COUNT]; // Font array for all text sizes

    // Performance monitoring
    float fps;              // Current frames per second
    clock_t startTime;      // Frame start time for FPS calculation
    clock_t previousTime;   // Previous frame time for delta calculations
} AppState;

/**
 * @brief Player state and movement parameters
 * 
 * Complete player state including position, movement speeds,
 * current selections, and debug display toggles.
 */
typedef struct {
    Player player;            // Position and orientation
    float rotateSpeed;        // Base rotation speed (degrees/second)
    float moveSpeed;          // Base movement speed (units/second)

    // Frame-rate adjusted speeds (calculated each frame)
    float playerRotateSpeed;  // Current frame rotation speed
    float playerMoveSpeed;    // Current frame movement speed
    
    int selectFrame;          // Currently selected inventory slot (1-7)

    // Debug display toggles
    bool showMap;             // Show minimap overlay
    bool showState;           // Show debug state interface
    bool showTextures;        // Enable texture rendering
    bool collision;           // Enable collision detection
} PlayerState;

/**
 * @brief Entity system state
 * 
 * Manages all 3D sprite entities in the game world including
 * the sprite array and depth buffer for proper rendering order.
 */
typedef struct {
    Sprite *sprites;    // Dynamic array of sprite entities
    int numSprites;     // Current number of active sprites
    float *zBuffer;     // Depth buffer for sprite sorting
} EntityState;

/**
 * @brief Menu system state and UI elements
 * 
 * Complete menu system state including current menu type,
 * background settings, and all button definitions for
 * various menu screens.
 */
typedef struct {
    int displayMenu;           // Current menu type (MenuType enum)
    BackgroundType backgroundType; // Background rendering style
    int scrollOffset;          // Vertical scroll position for long menus

    // Main menu buttons
    Button playButton;         // Start new game
    Button achievementsButton; // View achievements
    Button settingsButton;     // Open settings menu
    Button exitButton;         // Exit application

    // In-game pause menu buttons
    Button resumeGameButton;   // Resume current game
    Button exitGameButton;     // Return to main menu

    // Load game menu buttons
    Button loadGame1;          // Load save slot 1
    Button loadGame2;          // Load save slot 2
    Button loadGame3;          // Load save slot 3
    Button launchGame;         // Confirm and launch selected save

    // Navigation buttons
    Button returnButton;       // Return to previous menu
} MenuState;

/**
 * @brief Map and world state
 * 
 * Contains the game world map data, discovery state for fog-of-war,
 * and level configuration information.
 */
typedef struct {
    int mapWidth;        // Map width in grid cells
    int mapHeight;       // Map height in grid cells
    int **map;           // 2D array of tile types (walls, floors, etc.)
    int **mapDiscovered; // 2D array of discovery flags for each cell
    int typeLaunchGame;  // Game type/difficulty level
} MapState;

/**
 * @brief Graphics rendering buffers and caches
 * 
 * Contains all graphics-related buffers, textures, and cached
 * calculations used by the rendering pipeline for optimal performance.
 */
typedef struct {
    Uint32      *screenBuffers;        // Raw pixel buffer for raycasting
    SDL_Texture *screenBuffersTexture; // SDL texture for screen buffer
    Uint32     **textureBuffers;       // Loaded texture pixel data
    RenderCache renderCache;           // Pre-calculated rendering values
} GraphicsBuffers;

/**
 * @brief Game settings and configuration
 * 
 * User-configurable settings for gameplay, graphics, and audio.
 * Persisted to save files and applied across game sessions.
 */
typedef struct {
    float sensitivity; // Mouse sensitivity multiplier
    float fov;         // Field of view in degrees
    float volume;      // Master audio volume (0.0 - 1.0)
} Settings;

/**
 * @brief Complete game state structure
 * 
 * Master structure containing all game subsystem states.
 * This is the primary data structure passed between all modules.
 */
typedef struct {
    AppState app;              // Core application and SDL state
    PlayerState playerState;   // Player position and movement
    EntityState entityState;   // All 3D sprite entities
    MenuState menu;            // Menu system and UI elements
    MapState mapState;         // World map and discovery data
    GraphicsBuffers graphics;  // Rendering buffers and caches
    Settings settings;         // User preferences and configuration
} GameState;

// Global game state declaration
extern GameState gameState;

#endif /* MAIN_H */