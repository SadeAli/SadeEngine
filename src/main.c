#include "cglm/vec2.h"
#include <stdlib.h>
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
#define STB_PERLIN_IMPLEMENTATION
#include <stb/stb_perlin.h>

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
#include "shapes/grid.h"
#include "math/vec.h"

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

// TODO: make a verrtex struct which can hold various attributes (attributes will be bynamically added (before making it concreate))
/*
typedef struct DynamicVertex_t {
    VertexPosition position;
    u32 attributeCount;
    VertexAttribute *attributes;
} DynamicVertex;
*/

// IDEA: every object has its own hierarchy and objects will only hold indices to child objects
// TODO: systems

// TODO: townscaper style generation
#include "arrayDynamics.h"

typedef Vec3 VertexPosition;
typedef void VertexAttribute;

// TODO: Graph structures
typedef struct GraphNode_t GraphNode;
typedef struct GraphEdge_t GraphEdge;
// typedef struct GraphFace_t GraphFace;

struct HexagonalGraphNode_t {
    u32 connectionCount;
    u32 connectedNodes[6];
};

bool hexGridRelaxed() {
    const int layerCount = 4;
    const float layerDistance = 1.1;

    const int vertexCount = 3*layerCount*(layerCount + 1)+ 1;
    const u32 edgeCount = 3*layerCount*(3*layerCount + 1)*2;

    const u32 vertexBufferSize = sizeof(Vec2) * vertexCount;
    Vec2 *vertices = malloc(vertexBufferSize);
    u32 (*edges)[2] = malloc(edgeCount * sizeof(u32[2]));

    if ((vertices == nullptr) || (edges == nullptr)){
        free(vertices);
        free(edges);
        return false;
    }

    // place vertices
    vertices[0] = (Vec2){.data = {0,0}};
    int placedVertices = 1;
    // build hexagon layer by layer
    for (int i = 0; i < layerCount; i++) {
        // treat hexagon as 6 equal triangles
        // for each triangle
        for (int j = 0; j < 6; j++) {
            // create a starting point
            Vec2 edgeStart;
            glm_vec2_rotate((vec2){0, layerDistance * i + 1}, GLM_PI * j / 3, edgeStart.data);

            Vec2 target = edgeStart;

            Vec2 step;
            glm_vec2_rotate(edgeStart.data, GLM_PI * 2 / 3, step.data);
            glm_vec2_normalize(step.data);
            glm_vec2_scale(step.data, layerDistance, step.data);

            // travel trough the edge
            for (int k = 0; k < (i + 1); k++)
            {
                vertices[placedVertices] = target;
                glm_vec2_add(target.data, step.data, target.data);
                placedVertices += 1;
            }
        }
    }

    // create edges 
    // (think visually) select 2 points (I thought them as spiraling) and connect them
    int placedEdges = 0;
    // vertex index of the selected points

    u32 up = 1;
    u32 down = 0;
    for (int layer = 0; layer < layerCount; layer++)
    {
        for (int slice = 0; slice < 6; slice++)
        {
            // put initial edge
            edges[placedEdges][0] = down;
            edges[placedEdges][1] = up;
            placedEdges += 1;

            for (int i = 0; i < layer; i++)
            {
                up++;
                edges[placedEdges][0] = down;
                edges[placedEdges][1] = up;
                placedEdges += 1;

                down++;
                edges[placedEdges][0] = down;
                edges[placedEdges][1] = up;
                placedEdges += 1;
            }

            up++;
        }
        down++;
        
        // fix last edge of the frame
        if (layer > 0)
        {
            down--;
            edges[placedEdges - 1][0] -= 6*layer;
        }
    }

    // hexagonal frame
    for (int layer = 0; layer < layerCount; layer++)
    {
        u32 current =  3*layer*(layer + 1) + 1;
        for (int i = 0; i < 6*(layer+1); i++)
        {
            edges[placedEdges][0] = current;
            edges[placedEdges][1] = current + 1;
            placedEdges += 1;
            current++;
        }
        
        // fix last edge of the frame
        edges[placedEdges - 1][1] -= 6*(layer + 1);
    }

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, placedEdges * sizeof(u32[2]), (float*)edges, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(globalShader);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_LINES, placedEdges * 2, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    free(vertices);
    free(edges);
    return true;
}

bool drawLayeredHexagon() {
    const int layerCount = 4;
    const float layerDistance = 1.1;

    const int vertexCount = (layerCount + 1)*layerCount*3 + 1;
    const int edgeCount = ((3*layerCount*layerCount) - layerCount) * 3;

    const int triangleCount = layerCount * layerCount * 6;
    const int indexCount = triangleCount * 3;

    const u32 vertexBufferSize = sizeof(Vec2) * vertexCount;
    const u32 indexBufferSize = sizeof(u32) * indexCount;
    Vec2 *vertices = malloc(vertexBufferSize);
    int *indices = malloc(indexBufferSize);

    if ((vertices == nullptr) || (indices == nullptr)){
        free(vertices);
        free(indices);
        return false;
    }

    // place vertices
    //
    // place first vector
    vertices[0] = (Vec2){.data = {0,0}};
    int placedVertices = 1;

    // place the rest
    for (int i = 0; i < layerCount; i++) {
        // for each edge of the hexagon
        for (int j = 0; j < 6; j++) {
            Vec2 edgeStart;
            glm_vec2_rotate((vec2){0, layerDistance * i + 1}, GLM_PI * j / 3, edgeStart.data);

            Vec2 target = edgeStart;

            Vec2 step;
            glm_vec2_rotate(edgeStart.data, GLM_PI * 2 / 3, step.data);
            glm_vec2_normalize(step.data);
            glm_vec2_scale(step.data, layerDistance, step.data);

            // travel trough the edge
            for (int k = 0; k < (i + 1); k++)
            {
                vertices[placedVertices] = target;
                glm_vec2_add(target.data, step.data, target.data);
                placedVertices += 1;
            }
        }
    }

    // create triangles
    // -------------------------------------------------------------------------------------------
    // clockwise
    int passedVertices = 1;
    int placedIndices = 0;
    int up = 1;
    int down = 0;
    for (int i = 0; i < layerCount; i++)
    {
        int verticesInNextLayer = (i + 1) * 6;

        for (int j = 0; j < 6; j++)
        {
            // first triangle
            indices[placedIndices++] = up++;
            indices[placedIndices++] = down++;
            indices[placedIndices++] = up++;

            // generate the rest using strip method
            for (int k = 0; k < i; k++)
            {
                // strip
                indices[placedIndices] = indices[placedIndices - 1];
                indices[placedIndices + 1] = indices[placedIndices - 2];

                // new index
                indices[placedIndices + 2] = down++;
                placedIndices += 3;

                // strip
                indices[placedIndices] = indices[placedIndices - 3];
                indices[placedIndices + 1] = indices[placedIndices - 1];

                // new index
                indices[placedIndices + 2] = up++;
                placedIndices += 3;
            }

            up--;
            down--;
        }

        if (i != 0)
        {
            indices[placedIndices - 4] -= i * 6;
            indices[placedIndices - 2] -= i * 6;
        }
        indices[placedIndices - 1] -= (i + 1) * 6;

        down = passedVertices;
        passedVertices += verticesInNextLayer;
    }

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(globalShader);
    // glDrawElements(GL_LINE_STRIP, placedIndices, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, placedIndices, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    free(vertices);
    free(indices);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    return true;
}


// NOTE: main
int main(void)
{
    // window init
    Window window = init_windowDefault();

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
 
    ImGuiIO *ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls   

    // gl settings
    glEnable(GL_DEPTH_TEST);
    glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    glLineWidth(5);

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

    u32 cube = init_cube_vao_textured();

    assert(cube);
    assert(d.shader);
    assert(d.vao);

    // create matrices
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
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
        .position = {0, 5, 0},
        .target = {0,0,0},
        .up = {0,1,0},
    };

    // main loop
    bool demo = 1;
    while(!window_shouldClose(&window)) {

        // handle logic here
        {
            camera3d_updateOrbital(&camera, 0.3, 3);

            glUseProgram(d.shader);
            glUniformMatrix4fv(uView, 1, false, (float*)&camera.view);

            glUseProgram(helloShader);
            glUniformMatrix4fv(u2View, 1, false, (float*)&camera.view);

            glUseProgram(globalShader);
            glUniformMatrix4fv(u3View, 1, false, (float*)&camera.view);
        }

        // render settings
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
        }

        // render objects here
        {
            glUseProgram(d.shader);
            glad_glBindVertexArray(cube);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // // draw again with slight offset
            glm_translate(model, (vec3){1.2f, sin(glfwGetTime()), 0.0f});
            glUseProgram(d.shader);
            glUniformMatrix4fv(uModel, 1, false, (float*)&model);
            // glad_glBindVertexArray(cube);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // glDrawArrays(GL_LINE_STRIP, 0, 36);
            glm_translate(model, (vec3){-1.2f, -sin(glfwGetTime()), 0.0f});
            glUniformMatrix4fv(uModel, 1, false, (float*)&model);

            hexGridRelaxed();
            // drawLayeredHexagon();

            // drawGrid(helloShader);
            // drawGrid(helloShader);
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

