#include <raylib.h>
#include <raymath.h>

Camera3D createTPCamera() {
    return (Camera3D){
        .fovy = 60,
        .position = (Vector3){10, 0, 0},
        .target = Vector3Zero(),
        .projection = CAMERA_PERSPECTIVE,
        .up = (Vector3){0, 1, 0},
    };
}