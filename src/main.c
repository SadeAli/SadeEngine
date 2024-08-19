#define GLFW_INCLUDE_NONE

#include <assert.h>
#include <math.h>
#include <stdio.h>

// windowing
#include <GLFW/glfw3.h>

// vector/matrix operations
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
#include <GL/gl.h>

// GUI libs
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include <cimgui_impl.h>

// model loader libs
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// custom engine
#include "window.h"
#include "render/shader.h"
#include "render/drawable.h"
#include "defines.h"
#include "camera/camera3d.h"

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

// TODO: import and process 3D models
static bool import_example() {
    const struct aiScene* scene = aiImportFile( "resources/models/assimp-mdb/model-db/fbx/huesitos.fbx",
                aiProcess_CalcTangentSpace       |
                aiProcess_Triangulate            |
                aiProcess_JoinIdenticalVertices  |
                aiProcess_SortByPType);

    if( NULL == scene) {
        printf("%s", aiGetErrorString());
        return false;
    }

    // TODO: process mesh

    // NOTE: after import
    aiReleaseImport(scene);

    return true;
}

int main(void)
{
    // window init
    Window window = init_windowDefault();

    ShaderFile sList[] = {
        {"resources/shaders/texture_projection.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/texture_projection.vs", SHADER_TYPE_VERTEX},
    };
 
    ImGuiIO *ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls   

    // gl settings
    glEnable(GL_DEPTH_TEST);
    glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    glLineWidth(10);

    // NOTE:
    // init objects here

    // TODO: only allows for rendering with indices add all types of option
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
    glm_translate(view, (vec3){0.0f, 0.0f, 0.0f}); 
    glm_perspective(55, 16.0f/9.0f, 0.1f, 100.0f, projection);

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

    // initialize camera
    Camera3D camera = {
        .position = {0, 0, 0},
        .target = {0,0,0},
        .up = {0,1,0},
    };

    // main loop
    bool demo = 1;
    while(!window_shouldClose(&window)) {

        // handle logic here
        {
            camera3d_updateOrbital(&camera);
            glUniformMatrix4fv(uView, 1, false, (float*)&camera.view);
        }

        // render settings
        {
            glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
        }

        // render objects here
        {
            glUseProgram(d.shader);
            glUniformMatrix4fv(uModel, 1, false, (float*)&model);
            glad_glBindVertexArray(cube);
            // glDrawArrays(GL_TRIANGLES, 0, 36);

            // // draw again with slight offset
            // glm_translate(model, (vec3){1.2f, sin(glfwGetTime()), 0.0f});
            // glUseProgram(d.shader);
            // glUniformMatrix4fv(uModel, 1, false, (float*)&model);
            // glad_glBindVertexArray(cube);
            // glDrawArrays(GL_TRIANGLES, 0, 36);
            // glm_translate(model, (vec3){-1.2f, -sin(glfwGetTime()), 0.0f});

            glDrawArrays(GL_LINE_STRIP, 0, 36);
        }

        // render gui here
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            igNewFrame();
            // igShowDemoWindow(&demo);

            igRender();
            ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
        }

        // post render
        {
            window_swapBuffers(&window);
            window_pollEvents();
        }
    }

    // NOTE: clean resources
    glDeleteVertexArrays(1, &d.vao);
    glDeleteProgram(d.shader);

    // NOTE: terminate initialized systems
    window_close(&window);

    return 0;
}

