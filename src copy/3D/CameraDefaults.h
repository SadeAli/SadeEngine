#pragma once

#include <raylib.h>
#include <raymath.h>

static inline Camera3D createFreeCamera() {
    return (Camera3D){
        .fovy = 60,
        .position = (Vector3){10, 0, 0},
        .target = Vector3Zero(),
        .projection = CAMERA_PERSPECTIVE,
        .up = (Vector3){0, 1, 0},
    };
}