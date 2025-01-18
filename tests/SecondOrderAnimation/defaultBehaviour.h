#pragma once

#include <raylib.h>

#include "window.h"

#define INIT_WINDOW(...)                                                                                                \
    initWindow(                 \
        &(WindowSettings) {     \
            .width = 1920,      \
            .height = 1080,     \
            .fps = 60,          \
            .title = "myLeaf",  \
            __VA_ARGS__         \
        }                       \
    )

#define CAMERA3D_INIT(...)                      \
    (Camera3D) {                                \
        .position = {0, 0, 0},                  \
        .target = {0, 0, 0},                    \
        .up = {0, 1, 0},                        \
        .fovy = 60,                             \
        .projection = CAMERA_PERSPECTIVE,       \
        __VA_ARGS__                             \
    }

#define SCALE_INIT(...) \
    (Vector3) {         \
        .x = 1,         \
        .y = 1,         \
        .z = 1,         \
        __VA_ARGS__     \
    }

#define COLOR_INIT(...) \
    (Color) {           \
        .r = 0,         \
        .g = 0,         \
        .b = 0,         \
        .a = 255,       \
        __VA_ARGS__     \
    }
    