#include "graphics.h"

#define PI 3.14159265358979323846


float calculateFovRender(float fov) {
    return tanf((fov / 2.0f) * PI / 180.0f);
}

void calculateDirRender(float angle, float *dirX, float *dirY) {
    float dirAngle = angle * (PI / 180.0);
    *dirX = cos(dirAngle);
    *dirY = sin(dirAngle);
}