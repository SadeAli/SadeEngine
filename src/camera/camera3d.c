#include <cglm/cam.h>
#include <cglm/vec3.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "camera3d.h"
#include "cglm/types.h"
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <cglm/util.h>
#include <stdio.h>

// struct Vector3_t {float x, y, z;};

static inline Vector3 camera3d_up(Camera3D *cam) {
    return (Vector3) {
        .x = cam->view[0][1],
        .y = cam->view[1][1],
        .z = cam->view[2][1],
    };
}

static inline Vector3 camera3d_right(Camera3D *cam) {
    return (Vector3) {
        .x = cam->view[0][0],
        .y = cam->view[1][0],
        .z = cam->view[2][0],
    };
}
static inline Vector3 camera3d_forward(Camera3D *cam) {
    return (Vector3) {
        .x = cam->view[0][2],
        .y = cam->view[1][2],
        .z = cam->view[2][2],
    };
}

Camera3D construct_camera3d(Vector3 position, Vector3 target) {
    Camera3D cam;
    cam.position = position;
    glm_lookat((float *)&position, (float *)&target, (float[]){0,1,0}, cam.view);
    return cam;
}

void camera3d_lookat(Camera3D *cam, Vector3 target, Vector3 up) {
    Vector3 position = cam->position;
    glm_lookat((float*)&position, (float *)&target, (float *)&up, cam->view);
}

void camera3d_fly(Camera3D *cam, Vector3 displacement) {
    Vector3 forward = camera3d_forward(cam);
    Vector3 right = camera3d_right(cam);
    Vector3 up = camera3d_up(cam);

    Vector3 movement = {0};

    float *f = (float *)&forward;
    float *r = (float *)&forward;
    float *u = (float *)&forward;

    glm_vec3_scale(r, displacement.x, r);
    glm_vec3_scale(u, displacement.y, u);
    glm_vec3_scale(f, displacement.z, f);

    cam->position.x += forward.x + up.x + right.x;
    cam->position.y += forward.y + up.y + right.y;
    cam->position.z += forward.z + up.z + right.z;

    glm_lookat((float*)&cam->position, f, u, cam->view);
}


void camera3d_stabilizeUp(Camera3D *cam) {
    Vector3 up = (Vector3){0, -1, 0};

    cam->view[0][1] = up.x;
    cam->view[1][1] = up.y;
    cam->view[2][1] = up.z;
}

/*
void camera3d_recalculate(Camera3D *cam) {
    glm_vec3_normalize(cam->direction);
    glm_vec3_crossn(cam->up, cam->direction, cam->right);
    glm_vec3_cross(cam->direction, cam->right, cam->up);

    glm_lookat(cam->position, cam->target, cam->up, cam->view);
}
*/

// TODO: add function arguments
void camera3d_updateOrbital(Camera3D *cam, float rotSpeed, float radius) {
    Vector3 position = cam->position;
    glm_rotate_at(cam->view, (vec3){0, position.y, 0}, glm_rad(0.5), (vec3){0,1,0});
    return;

    /*
    float camX = sin(glfwGetTime() * rotSpeed) * radius;
    float camZ = cos(glfwGetTime() * rotSpeed) * radius;

    cam->position[0] = camX;
    cam->position[2] = camZ;

    glm_vec3_sub(cam->target, cam->position, cam->direction);
    
    camera3d_recalculate(cam);
    */

}

void camera3d_updateFirstPerson(Camera3D *cam, float yaw, float pitch) {
    Vector3 position = cam->position;

    Vector3 right = camera3d_right(cam);
    glm_vec3_negate((float*)&right);

    glm_rotate_at(cam->view, (float *)&position, glm_rad(pitch), (float*)&right);
    glm_rotate_at(cam->view, (float *)&position, glm_rad(yaw), (vec3){0,1,0});

    /*
    printf("---------------------\n"
            "%.2f %.2f %.2f %.2f\n"
           "%.2f %.2f %.2f %.2f\n"
           "%.2f %.2f %.2f %.2f\n"
           "%.2f %.2f %.2f %.2f\n"
           , cam->view[0][0], cam->view[0][1], cam->view[0][2], cam->view[0][3]
           , cam->view[1][0], cam->view[1][1], cam->view[1][2], cam->view[1][3]
           , cam->view[2][0], cam->view[2][1], cam->view[2][2], cam->view[2][3]
           , cam->view[3][0], cam->view[3][1], cam->view[3][2], cam->view[3][3]);
    */
}

void camera3d_lookAt(Camera3D *cam, vec3 target) {
    Vector3 up = camera3d_up(cam);
    Vector3 position = cam->position;
    glm_lookat((float*)&position, target, (float*)&up, cam->view);
}

