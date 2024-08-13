#define GLFW_INCLUDE_NONE

#include <assert.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "glad/glad.h"
#include "render/shader.h"
#include "render/drawable.h"

#include "defines.h"

#include <math.h>

#include "cglm/mat4.h"
#include "cglm/types.h"
#include "cglm/affine.h"
#include "cglm/affine-pre.h"
#include "cglm/cam.h"
#include "cglm/clipspace/persp_lh_no.h"
#include "cglm/util.h"

#include "stb/stb_image.h"

struct Engine
{
    WindowSettings windowSettings;
} engine = {
    .windowSettings =
        {
            .fps = 60,
            .height = 1080,
            .width = 1920,
            .title = "myLeaf",
        },
};


int main(void)
{
    Window window = init_windowDefault();

    ShaderFile sList[] = {
        {"resources/shaders/texture_projection.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/texture_projection.vs", SHADER_TYPE_VERTEX},
    };

    glEnable(GL_DEPTH_TEST);

    // WARN: only allows for rendering with indices
    Drawable d = {
        .vao = init_rect_vao(),
        .elementCount = 6,
        .shader = init_openglShaderProgram(sList, 2),
    };

    u32 cube = init_cube_vao_textured();

    assert(cube);
    assert(d.shader);
    assert(d.vao);

    // create matrices
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(model, glm_rad(-10), (float[]){1,0,0});
    glm_translate(view, (vec3){0.0f, 0.0f, -2.0f}); 
    glm_perspective(55, 16.0f/9.0f, 0.1f, 100.0f, projection);

        // move data to shader
    glUseProgram(d.shader);
    int uProjection = glGetUniformLocation(d.shader, "uProjection");
    int uView = glGetUniformLocation(d.shader, "uView");
    int uModel = glGetUniformLocation(d.shader, "uModel");

    glUniformMatrix4fv(uView, 1, false, (float*)&view);
    glUniformMatrix4fv(uProjection, 1, false, (float*)&projection);
    glUniformMatrix4fv(uModel, 1, false, (float*)&model);

    glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    while(!window_shouldClose(&window)) {
        // NOTE: handle logic here

        // render start
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm_rotate(model, glm_rad(-1), (float[]){0.17,0.13,0.11});

        // NOTE: render here
        // drawable_draw(d);
        glUseProgram(d.shader);
        glUniformMatrix4fv(uModel, 1, false, (float*)&model);
        glad_glBindVertexArray(cube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw again with slight offset
        glm_translate(model, (vec3){1.2f, sin(glfwGetTime()), 0.0f}); 
        glUseProgram(d.shader);
        glUniformMatrix4fv(uModel, 1, false, (float*)&model);
        glad_glBindVertexArray(cube);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glm_translate(model, (vec3){-1.2f, -sin(glfwGetTime()), 0.0f}); 

        // render end
        window_swapBuffers(&window);
        window_pollEvents();
    }

    glDeleteVertexArrays(1, &d.vao);
    glDeleteProgram(d.shader);

    // TODO: maybe change name
    window_terminate(&window);

    return 0;
}

