#pragma once

#include <raylib.h>

#define INIT_CAMERA3D(...)                      \
    (Camera3D) {                                \
        .position = {0, 0, 0},                  \
        .target = {0, 0, 0},                    \
        .up = {0, 1, 0},                        \
        .fovy = 60,                             \
        .projection = CAMERA_PERSPECTIVE,       \
        __VA_ARGS__                             \
    }

void updateCameraCustom(Camera3D *camera, float moveSpeed, float rotationSpeed,
                        float rollSpeed);