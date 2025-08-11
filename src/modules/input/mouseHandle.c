#include "input.h"

static int validateMouseEvent(const GameState *state, const SDL_Event event) {
    // Suppress unused parameter warning for placeholder validation
    (void)event;
    
    if (!state) {
        return -1;
    }
    
    return 0;
}

static void processMouseLook(GameState *state, int xrel) {
    // Apply mouse sensitivity
    float rotationAmount = xrel * state->settings.sensitivity;
    
    state->playerState.player.angle += rotationAmount;
    
    // Normalize angle to [0, 360) range
    while (state->playerState.player.angle < 0.0f) {
        state->playerState.player.angle += 360.0f;
    }
    while (state->playerState.player.angle >= 360.0f) {
        state->playerState.player.angle -= 360.0f;
    }
    
    // Update render cache with new direction
    calculateDirRender(state->playerState.player.angle, 
                      &state->graphics.renderCache.dirX, 
                      &state->graphics.renderCache.dirY);
}

static void processMouseWheel(GameState *state, int wheelY) {
    const int MIN_FRAME = 1;
    const int MAX_FRAME = 7;
    const int SCROLL_STEP = 20;
    
    if (wheelY > 0) {
        // Scroll up / Previous frame
        if (state->playerState.selectFrame > MIN_FRAME) {
            state->playerState.selectFrame -= 1;
        }
        
        // Menu scroll up (dynamic clamp)
        if (state->menu.scrollOffset > 0) {
            int contentHeight = 0;
            if (state->menu.displayMenu == MENU_ACHIEVEMENTS) {
                const int achievementHeight = 125;
                const int achievementSpacing = 20;
                int count = state->achievements.count;
                contentHeight = 100 + (achievementHeight + achievementSpacing) * count;
            }
            int viewport = state->app.screenHeight - 100;
            int maxOffset = (contentHeight > viewport) ? (contentHeight - viewport) : 0;
            int next = state->menu.scrollOffset - SCROLL_STEP;
            state->menu.scrollOffset = (next < 0) ? 0 : next;
            if (state->menu.scrollOffset > maxOffset) state->menu.scrollOffset = maxOffset;
        }
    } else if (wheelY < 0) {
        // Scroll down / Next frame
        if (state->playerState.selectFrame < MAX_FRAME) {
            state->playerState.selectFrame += 1;
        }
        
        // Menu scroll down (dynamic clamp)
        int contentHeight = 0;
        if (state->menu.displayMenu == MENU_ACHIEVEMENTS) {
            const int achievementHeight = 125;
            const int achievementSpacing = 20;
            int count = state->achievements.count;
            contentHeight = 100 + (achievementHeight + achievementSpacing) * count;
        }
        int viewport = state->app.screenHeight - 100;
        int maxOffset = (contentHeight > viewport) ? (contentHeight - viewport) : 0;
        int next = state->menu.scrollOffset + SCROLL_STEP;
        state->menu.scrollOffset = (next > maxOffset) ? maxOffset : next;
    }
}

static void processMouseClick(GameState *state, const SDL_Event event) {
    if (event.button.button != SDL_BUTTON_LEFT) {
        return; // Only handle left clicks for now
    }
    
    if (state->menu.displayMenu == MENU_NONE) {
        return; // No menu interaction needed during gameplay
    }
    
    int mouseX = event.button.x;
    int mouseY = event.button.y;
    
    // Validate click coordinates
    if (mouseX < 0 || mouseX >= state->app.screenWidth ||
        mouseY < 0 || mouseY >= state->app.screenHeight) {
        return;
    }
    
    // Handle menu button clicks
    handleMenuButtons(state, mouseX, mouseY);
}

void mouseHandle(GameState *state, const SDL_Event event) {
    // Validate input parameters
    if (validateMouseEvent(state, event) != 0) {
        return;
    }
    
    switch (event.type) {
        case SDL_MOUSEMOTION:
            // Handle mouse look only during gameplay
            if (state->menu.displayMenu == MENU_NONE) {
                processMouseLook(state, event.motion.xrel);
            }
            break;
            
        case SDL_MOUSEWHEEL:
            // Handle mouse wheel for frame selection and menu scrolling
            processMouseWheel(state, event.wheel.y);
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            // Handle mouse button clicks for UI interaction
            processMouseClick(state, event);
            break;
            
        default:
            // Unhandled mouse event type
            break;
    }
}