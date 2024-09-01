#include <cglm/cam.h>
#include <cglm/vec3.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "camera3d.h"
#include "cglm/types.h"
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <cglm/util.h>

typedef struct Vector3_t {float x, y, z;} Vector3;

void camera3d_stabilizeUp(Camera3D *cam) {
    *(Vector3*)cam->up = (Vector3){0, -1, 0};

    cam->view[0][1] = cam->up[0];
    cam->view[1][1] = cam->up[1];
    cam->view[2][1] = cam->up[2];
}

void camera_updateFromView(Camera3D *cam) {
  cam->right[0] = cam->view[0][0];
  cam->right[1] = cam->view[1][0];
  cam->right[2] = cam->view[2][0];

  cam->up[0] = cam->view[0][1];
  cam->up[1] = cam->view[1][1];
  cam->up[2] = cam->view[2][1];
}

void camera3d_recalculate(Camera3D *cam) {
    glm_vec3_normalize(cam->direction);
    glm_vec3_crossn(cam->up, cam->direction, cam->right);
    glm_vec3_cross(cam->direction, cam->right, cam->up);

    glm_lookat(cam->position, cam->target, cam->up, cam->view);
}

// TODO: add function arguments
void camera3d_updateOrbital(Camera3D *cam, float rotSpeed, float radius) {
    glm_rotate_at(cam->view, (vec3){0, cam->position[1] ,0}, glm_rad(0.5), (vec3){0,1,0});
    camera_updateFromView(cam);
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
    glm_rotate_at(cam->view, cam->position, glm_rad(pitch), cam->right);
    glm_rotate_at(cam->view, cam->position, glm_rad(yaw), (vec3){0,-1,0});
    camera_updateFromView(cam);
}

void camera3d_lookAt(Camera3D *cam, vec3 target) {
    glm_lookat(cam->position, cam->target, cam->up, cam->view);

    camera_updateFromView(cam);
}

