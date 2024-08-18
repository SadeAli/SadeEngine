#include "io/file.h"
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
    // NOTE:
    // window init
    Window window = init_windowDefault();

    ShaderFile sList[] = {
        {"resources/shaders/texture_projection.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/texture_projection.vs", SHADER_TYPE_VERTEX},
    };
 
    ImGuiIO *ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls   

    // NOTE:
    // init
    glEnable(GL_DEPTH_TEST);
    glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);


    // NOTE:
    // init objects here

    // WARN:
    // only allows for rendering with indices
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

    // TODO:
    // implement Camera3D struct

    typedef struct Camera3D_t {
        vec3 position;
        vec3 target;
        vec3 direction;

        vec3 up;
        vec3 right;
    } Camera3D;

    // initialize camera
    Camera3D camera = {
        .position = {0, 0, 10},
        .target = {0,0,0},
        .up = {0,1,0},
    };

    // calculate directions
    glm_vec3_sub(camera.target, camera.position, camera.direction);
    glm_vec3_normalize(camera.direction);
    glm_vec3_crossn(camera.up, camera.direction, camera.right);
    glm_vec3_cross(camera.direction, camera.right, camera.up);

    // TODO:
    // model importer

    const struct aiScene* scene = aiImportFile( "resources/models/assimp-mdb/model-db/fbx/huesitos.fbx",
                aiProcess_CalcTangentSpace       |
                aiProcess_Triangulate            |
                aiProcess_JoinIdenticalVertices  |
                aiProcess_SortByPType);

    if( NULL == scene) {
        printf("%s", aiGetErrorString());
        return 1;
    }

    // TODO: mesh processing

    
    // NOTE: after import
    aiReleaseImport( scene);

    // NOTE: main loop
    bool demo = 1;
    while(!window_shouldClose(&window)) {
        // NOTE:
        // handle logic here

        const float radius = 5.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        camera.position[0] = camX;
        camera.position[2] = camZ;

        // NOTE: this part is for camera update
        // calculate directions
        glm_vec3_sub(camera.target, camera.position, camera.direction);
        glm_vec3_normalize(camera.direction);
        glm_vec3_crossn(camera.up, camera.direction, camera.right);
        glm_vec3_cross(camera.direction, camera.right, camera.up);

        glm_lookat(camera.position, camera.target, camera.up, view);

        glUniformMatrix4fv(uView, 1, false, (float*)&view);

        // glm_rotate(model, glm_rad(-1), (float[]){0.17,0.13,0.11});

        
        // NOTE:
        // render settings
        glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);

        // NOTE:
        // render objects here
        
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
       

        // NOTE: 
        // render gui here
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        igNewFrame();
        // igShowDemoWindow(&demo);



        igRender();
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());


        // NOTE:
        // post render
        window_swapBuffers(&window);
        window_pollEvents();
    }

    // NOTE: clean resources
    glDeleteVertexArrays(1, &d.vao);
    glDeleteProgram(d.shader);

    // NOTE:
    // terminate initialized systems
    window_close(&window);

    return 0;
}

