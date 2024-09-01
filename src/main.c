#include "assimp/mesh.h"
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
#define STB_PERLIN_IMPLEMENTATION
#include <stb/stb_perlin.h>

// opengl loader
#include <glad/glad.h>
#include <GL/gl.h>

// GUI libs
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

// model loader libs
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// custom engine
#include "window.h"
#include "gui/gui.h"
#include "render/shader.h"
#include "render/drawable.h"
#include "defines.h"
#include "camera/camera3d.h"
#include "glShapes.h"
#include "math/vec.h"

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

static OpenglShader globalShader = 0;

// TODO: import and process 3D models
Drawable import_example() {
    const struct aiScene* scene = aiImportFile( "resources/models/assimp-mdb/model-db/fbx/huesitos.fbx",
                aiProcess_CalcTangentSpace       |
                aiProcess_Triangulate            |
                aiProcess_JoinIdenticalVertices  |
                aiProcess_SortByPType);

    if( NULL == scene) {
        printf("%s", aiGetErrorString());
        return (Drawable){0};
    }

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    long totalVertexSize = 0;
    for (u32 i = 0; i < scene->mNumMeshes; i++) {
        totalVertexSize += scene->mMeshes[i]->mNumVertices * sizeof(*scene->mMeshes[i]->mVertices);
    }

    // Allocate buffer memory
    glBufferData(GL_ARRAY_BUFFER, totalVertexSize, NULL, GL_STATIC_DRAW);

    // Calculate total size of element buffer
    long totalElementSize = 0;
    for (u32 i = 0; i < scene->mNumMeshes; i++) {
        for (u32 j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
            totalElementSize += scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(*scene->mMeshes[i]->mFaces[j].mIndices);
        }
    }

    // Allocate buffer memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalElementSize, NULL, GL_STATIC_DRAW);

    // TODO: process mesh
    long vboOffset = 0;
    for (u32 i = 0; i < scene->mNumMeshes; i++)
    {
        struct aiMesh *mesh = scene->mMeshes[i];
        glBufferSubData(GL_ARRAY_BUFFER,
                        vboOffset,
                        mesh->mNumVertices * sizeof(*mesh->mVertices),
                        mesh->mVertices);
        vboOffset += mesh->mNumVertices * sizeof(*mesh->mVertices);
    }

    long eboOffset = 0;
    for (u32 i = 0; i < scene->mNumMeshes; i++)
    {
        const struct aiMesh mesh = *scene->mMeshes[i];
        for (u32 j = 0; j < mesh.mNumFaces; j++)
        {
            const struct aiFace face = mesh.mFaces[j];
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                            eboOffset,
                            face.mNumIndices * sizeof(*face.mIndices),
                            face.mIndices);
            eboOffset += face.mNumIndices * sizeof(*face.mIndices);
        }
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
    glEnableVertexAttribArray(0);

    glDeleteBuffers(scene->mNumMeshes, &vbo);
    glDeleteBuffers(scene->mNumMeshes, &ebo);

    // NOTE: after import
    aiReleaseImport(scene);

    GLenum error = 0;
    while ((error = glGetError())) {
        printf("error: %d\n", error);
    }

    return (Drawable){.vao = vao, .elementCount = eboOffset};
}

// TODO: make a verrtex struct which can hold various attributes (attributes will be bynamically added (before making it concreate))

// IDEA: every object has its own hierarchy and objects will only hold indices to child objects
// TODO: systems


// NOTE: main
int main(void)
{
    // window init
    Window window = init_windowDefault();
    window_hideCursor(&window);

    ShaderFile sList[] = {
        {"resources/shaders/texture_projection.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/texture_projection.vs", SHADER_TYPE_VERTEX},
    };

    ShaderFile s2List[] = {
        {"resources/shaders/hello.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/projection.vs", SHADER_TYPE_VERTEX},
        //{"resources/shaders/projection.vs", SHADER_TYPE_VERTEX},
    };

    ShaderFile s3List[] = {
        {"resources/shaders/hello.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/texture_projection_2dto3d.vs", SHADER_TYPE_VERTEX},
    };

    ShaderFile s4List[] = {
        {"resources/shaders/hello.fs", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/2d.vs", SHADER_TYPE_VERTEX}
    };

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
        .shader = init_openglShaderProgram(sList, 2),
    };

    OpenglShader helloShader = init_openglShaderProgram(s2List, 2);
    globalShader = init_openglShaderProgram(s3List, 2);
    OpenglShader shade2d = init_openglShaderProgram(s4List, 2);
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

        // move data to shader
    glUseProgram(globalShader);
    int u3Projection = glGetUniformLocation(helloShader, "uProjection");
    int u3View = glGetUniformLocation(helloShader, "uView");
    int u3Model = glGetUniformLocation(helloShader, "uModel");

    glUniformMatrix4fv(u3View, 1, false, (float*)&view);
    glUniformMatrix4fv(u3Projection, 1, false, (float*)&projection);
    glUniformMatrix4fv(u3Model, 1, false, (float*)&model);

    // initialize camera
    Camera3D camera = {
        .position = {0, 5, 5},
        .target = {0,0,0},
        .up = {0,-1,0},
    };

    Drawable model_drawable = import_example();
    model_drawable.shader = globalShader;

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

            glUseProgram(globalShader);
            glUniformMatrix4fv(u3View, 1, false, (float*)camera.view);
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

            // drawable_draw(model_drawable);
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
    glDeleteVertexArrays(1, &d.vao);
    glDeleteProgram(d.shader);

    // NOTE: terminate initialized systems
    window_close(&window);

    return 0;
}

