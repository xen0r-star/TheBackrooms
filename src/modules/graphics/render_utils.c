#include "graphics.h"
#include <math.h>


float calculateFovRender(float fov) {
    // Validate FOV range
    if (fov <= 0.0f || fov >= 180.0f) {
        printf("Warning: Invalid FOV %.1f°, clamping to 75°\n", fov);
        fov = 75.0f; // Default safe FOV
    }
    
    // Convert degrees to radians and calculate tangent
    float fovRadians = (fov / 2.0f) * (PI / 180.0f);
    return tanf(fovRadians);
}

void calculateDirRender(float angle, float *dirX, float *dirY) {
    // Validate input parameters
    if (!dirX || !dirY) {
        return;
    }
    
    // Normalize angle to [0, 360) range
    while (angle < 0.0f) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;
    
    // Convert degrees to radians
    float dirAngle = angle * (PI / 180.0f);
    
    // Calculate direction components
    *dirX = cosf(dirAngle);
    *dirY = sinf(dirAngle);
    
    // Ensure vectors are normalized (should be automatic with sin/cos)
    float magnitude = sqrtf((*dirX) * (*dirX) + (*dirY) * (*dirY));
    if (magnitude > 0.0f && fabs(magnitude - 1.0f) > 0.001f) {
        *dirX /= magnitude;
        *dirY /= magnitude;
    }
}