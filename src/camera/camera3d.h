#pragma once

typedef struct Vector3_t Vector3;
struct Vector3_t {
    float x, y, z;
};

#include <cglm/types.h>

typedef struct Camera3D_t {
    Vector3 position;
    mat4 view;
} Camera3D;

Camera3D construct_camera3d(Vector3 position, Vector3 target);
void camera3d_recalculate(Camera3D *cam);
void camera3d_lookat(Camera3D *cam, Vector3 target, Vector3 up);
void camera3d_updateOrbital(Camera3D *cam, float rotSpeed, float radius);
void camera3d_updateFirstPerson(Camera3D *cam, float yaw, float pitch);
void camera3d_fly(Camera3D *cam, Vector3 displacement);
