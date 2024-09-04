#define GLFW_INCLUDE_NONE

// stdlib
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

// vector/matrix operations
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <cglm/cam.h>
#include <cglm/clipspace/persp_lh_no.h>
#include <cglm/util.h>

// image loader
#include <stb/stb_image.h>

// opengl loader
#include <glad/glad.h>

// GUI libs
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

// model loader libs
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

// custom engine
#include "window.h"
#include "gui.h"
#include "shader.h"
#include "render/drawable.h"
#include "defines.h"
#include "camera/camera3d.h"
#include "glShapes.h"
#include "math/vec.h"
#include "modelLoader.h"

struct Vector2_t {
    float x, y;
};

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

// TODO: make a verrtex struct which can hold various attributes (attributes will be bynamically added (before making it concreate))

// IDEA: every object has its own hierarchy and objects will only hold indices to child objects
// TODO: systems


// NOTE: main
int main(void)
{
    // window init
    Window window = init_windowDefault();
    window_hideCursor(&window);

    Shader fs_textureProjection = construct_shader("resources/shaders/texture_projection.fs", SHADER_TYPE_FRAGMENT);
    Shader vs_textureProjection = construct_shader("resources/shaders/texture_projection.vs", SHADER_TYPE_VERTEX);

    Shader fs_hello = construct_shader("resources/shaders/hello.fs", SHADER_TYPE_FRAGMENT);
    Shader vs_projection = construct_shader("resources/shaders/projection.vs", SHADER_TYPE_VERTEX);

    Shader vs_2d = construct_shader("resources/shaders/2d.vs", SHADER_TYPE_VERTEX);

    // gl settings
    glEnable(GL_DEPTH_TEST);
    glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    glLineWidth(2);

    // NOTE:
    // init objects here

    // TODO: only allows for rendering with indices add all types of option
    Drawable d = {
        .vao = init_rect_vao(),
        .elementCount = 6,
        .shader = construct_shaderProgram((Shader[]){fs_textureProjection, vs_textureProjection}, 2),
    };

    ShaderProgram helloShader = construct_shaderProgram((Shader[]){fs_hello, vs_projection}, 2);
    ShaderProgram shade2d = construct_shaderProgram((Shader[]){fs_hello, vs_2d}, 2);
    u32 cube = init_cube_vao_textured();

    assert(cube);
    assert(d.shader);
    assert(shade2d);
    assert(d.vao);

    // create matrices
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(55, 16.0f/9.0f, 0.01f, 1000.0f, projection);

    // move data to shader
    glUseProgram(d.shader);
    int uProjection = glGetUniformLocation(d.shader, "uProjection");
    int uView = glGetUniformLocation(d.shader, "uView");
    int uModel = glGetUniformLocation(d.shader, "uModel");

    glUniformMatrix4fv(uView, 1, false, (float*)&view);
    glUniformMatrix4fv(uProjection, 1, false, (float*)&projection);
    glUniformMatrix4fv(uModel, 1, false, (float*)&model);

    glUniform1i(glGetUniformLocation(d.shader, "texture1"), 0);
    glUniform1i(glGetUniformLocation(d.shader, "texture2"), 1);

    // move data to shader
    glUseProgram(helloShader);
    int u2Projection = glGetUniformLocation(helloShader, "uProjection");
    int u2View = glGetUniformLocation(helloShader, "uView");
    int u2Model = glGetUniformLocation(helloShader, "uModel");

    glUniformMatrix4fv(u2View, 1, false, (float*)&view);
    glUniformMatrix4fv(u2Projection, 1, false, (float*)&projection);
    glUniformMatrix4fv(u2Model, 1, false, (float*)&model);

    // initialize camera
    Camera3D camera = {
        .position = {0, 5, 5},
        .target = {0,0,0},
        .up = {0,1,0},
    };

    Drawable model_drawable = import_test();

    assert(model_drawable.vao);

    // main loop
    bool demo = 1;
    bool request = 0;
    glm_lookat(camera.position, camera.target, camera.up, camera.view);
    while(!window_shouldClose(&window)) {

        // handle logic here
        {
            // camera3d_updateOrbital(&camera, 0.3, 3);
            float sensivity = 0.2;
            const Vector2 mouseDelta = window_getMouseDelta(&window);
            camera3d_updateFirstPerson(&camera, mouseDelta.x * sensivity, mouseDelta.y * sensivity);

            glUseProgram(d.shader);
            glUniformMatrix4fv(uView, 1, false, (float*)camera.view);

            glUseProgram(helloShader);
            glUniformMatrix4fv(u2View, 1, false, (float*)camera.view);
        }

        // render settings
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
        }

        // render objects here
        {
            drawGrid(helloShader);

            if (request) {
                glUseProgram(d.shader);
                glBindVertexArray(cube);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glUseProgram(shade2d);
            // drawRect((Vec2){10, 10}, (Vec2){100, 100});

            model_drawable.shader = d.shader;
            drawable_draw(model_drawable);
        }

        // render gui here
        guiBegin();
        {
            bool open = 1;
            if (igBegin("tools", &open, ImGuiConfigFlags_None)) 
            {
                if (igButton("add cube", (ImVec2){0, 0})) {
                    request = !request;
                }
            }
            igEnd();

            igShowDemoWindow(&demo);
        }
        guiEnd();

        // post render
        {
            window_swapBuffers(&window);
            window_pollEvents();
        }
    }

    // NOTE: clean resources
    glDeleteVertexArrays(1, &model_drawable.vao);
    glDeleteVertexArrays(1, &d.vao);
    glDeleteProgram(d.shader);

    // NOTE: terminate initialized systems
    window_close(&window);

    return 0;
}

