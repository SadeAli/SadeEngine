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

static inline Vector3 camera3d_up(Camera3D *cam) {
    return (Vector3) {
        .x = cam->view[0][1],
        .y = cam->view[1][1],
        .z = cam->view[2][1],
    };
}

static inline Vector3 camera3d_right(Camera3D *cam) {
    return (Vector3) {
        .x = -cam->view[0][0],
        .y = -cam->view[1][0],
        .z = -cam->view[2][0],
    };
}
static inline Vector3 camera3d_forward(Camera3D *cam) {
    return (Vector3) {
        .x = -cam->view[0][2],
        .y = -cam->view[1][2],
        .z = -cam->view[2][2],
    };
}

Camera3D construct_camera3d(Vector3 position, Vector3 target) {
    Camera3D cam;
    cam.position = position;
    camera3d_lookat(&cam, target, (Vector3){0,-1,0});
    return cam;
}

void camera3d_lookat(Camera3D *cam, Vector3 target, Vector3 up) {
    Vector3 position = cam->position;
    glm_lookat((float*)&position, (float *)&target, (float *)&up, cam->view);
}

void camera3d_fly(Camera3D *cam, Vector3 displacement) {
    const Vector3 forward = camera3d_forward(cam);
    const Vector3 right = camera3d_right(cam);
    const Vector3 up = camera3d_up(cam);

    Vector3 forwardDisplacement = camera3d_forward(cam);
    Vector3 rightDisplacement = camera3d_right(cam);
    Vector3 upDisplacement = camera3d_up(cam);

    // Scale movement vectors by displacement
    glm_vec3_scale((float*)&right, displacement.x, (float*)&rightDisplacement);
    glm_vec3_scale((float*)&up, -displacement.y, (float*)&upDisplacement);
    glm_vec3_scale((float*)&forward, displacement.z, (float*)&forwardDisplacement);

    // Combine the movements
    Vector3 movement = {
        forwardDisplacement.x + upDisplacement.x + rightDisplacement.x,
        forwardDisplacement.y + upDisplacement.y + rightDisplacement.y,
        forwardDisplacement.z + upDisplacement.z + rightDisplacement.z
    };

    // Apply movement to the camera's position
    glm_vec3_add((float*)&cam->position, (float*)&movement, (float*)&cam->position);

    // Recalculate the camera's forward and up vectors and adjust the look direction
    Vector3 target = {
        cam->position.x + forward.x,
        cam->position.y + forward.y,
        cam->position.z + forward.z
    };

    glm_look((float*)&cam->position, (float*)&forward, (float*)&up, cam->view);

    return;

    /* TODO: fix camera up direction
     * printf("---------\n");
     * printf("%f %f %f \n", cam->position.x, cam->position.y, cam->position.z);
     * printf("%f %f %f \n", up.x, up.y, up.z);
    */
}

void camera3d_stabilizeUp(Camera3D *cam) {
    Vector3 up = (Vector3){0, 1, 0};

    // write up direction back to view matrix
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

    glm_rotate_at(cam->view, (float *)&position, glm_rad(pitch), (float*)&right);
    glm_rotate_at(cam->view, (float *)&position, glm_rad(yaw), (vec3){0,1,0});
}

void camera3d_lookAt(Camera3D *cam, Vector3 target) {
    Vector3 up = camera3d_up(cam);
    Vector3 position = cam->position;
    glm_lookat((float*)&position, (float*)&target, (float*)&up, cam->view);
}

