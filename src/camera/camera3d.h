#pragma once

#include <cglm/types.h>
#include <cglm/vec3.h>

typedef struct Camera3D_t {
    vec3 position;
    vec3 target;
    vec3 direction;

    vec3 up;
    vec3 right;

    mat4 view;
} Camera3D;

void camera3d_recalculate(Camera3D *cam);
void camera3d_updateOrbital(Camera3D *cam, float rotSpeed, float radius);

