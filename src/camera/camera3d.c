#include <cglm/cam.h>
#include <cglm/vec3.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "camera3d.h"

void camera3d_recalculate(Camera3D *cam) {
    // TODO: make a better calculation
    cam->up[0] = 0;
    cam->up[1] = -1;
    cam->up[2] = 0;

    glm_vec3_sub(cam->target, cam->position, cam->direction);
    glm_vec3_normalize(cam->direction);
    glm_vec3_crossn(cam->up, cam->direction, cam->right);
    glm_vec3_cross(cam->direction, cam->right, cam->up);

    glm_lookat(cam->position, cam->target, cam->up, cam->view);
}

// TODO: add function arguments
void camera3d_updateOrbital(Camera3D *cam, float rotSpeed, float radius) {
    float camX = sin(glfwGetTime() * rotSpeed) * radius;
    float camZ = cos(glfwGetTime() * rotSpeed) * radius;

    cam->position[0] = camX;
    cam->position[2] = camZ;

    camera3d_recalculate(cam);
}
