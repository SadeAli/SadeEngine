
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "resource/image/image.h"
#include <math.h>
#include <shader.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sadeEngine.h>
#include <math/vector/vector2.h>
#include <math/vector/vector3.h>
#include <sadeGL.h>
#include <defines.h>
#include <shape_generator.h>
#include <mesh2d.h>
#include <mesh.h>

void initOpenGLSettings(void);
Window createWindow(void);

int main(void) {
    // Create Window
    Window window = createWindow();
    initOpenGLSettings();

    // Load Shaders
    ShaderProgram shaderProgram = loadShaderProgram("../resources/shaders/default.vs", "../resources/shaders/default.fs");
    assert(shaderProgram);
    shaderProgram_setFloat(shaderProgram, "aspect_ratio", (float)window.settings.width / (float)window.settings.height);

    // Generate Vertices and Indices
    int segmentCount = 39; // Necati Tarafindan Yuvarlaklik Onayli
    size_t vCount = segmentCount + 1;
    size_t iCount = 3 * segmentCount;
    float radius = 0.2f;
    Vector2 *vertexPositions = malloc(vCount * sizeof(Vector2));
    Vector3 *vertexColors = malloc(vCount * sizeof(Vector3));
    uint *indices = malloc(iCount * sizeof(uint));
    assert(vertexPositions && indices);
    generateCircleVertices2D(vertexPositions, segmentCount, radius);
    generateCircleIndices(indices, segmentCount);

    for (uint i = 0; i < vCount; i++) {
        // vertexColors[i].x = (float)(rand() % 10000) / 10000;
        // vertexColors[i].y = (float)(rand() % 10000) / 10000;
        // vertexColors[i].z = (float)(rand() % 10000) / 10000;
        
        vertexColors[i] = (Vector3){0.99, 0.99, 0.90};
    }

    Image image = loadImage("../resources/sprites/sadeAli.png");
    assert(image.data);

    Mesh mesh = {0};
    mesh_addVertexAttribute(&mesh, 0, 2, GL_FLOAT, GL_FALSE); // position 2D
    mesh_addVertexAttribute(&mesh, 1, 3, GL_FLOAT, GL_FALSE); // color RGB

    // TODO:
    assert(mesh_addAttribute(&mesh, "position", 2, GL_FLOAT)); // transform

    mesh.vertexCount = vCount;
    mesh.indexCount = iCount;
    mesh.indices = indices;
    mesh.shaderProgram = shaderProgram;
    mesh_assignInterleavedData(&mesh, (void*[]){vertexPositions, vertexColors});

    mesh_uploadToGPU(&mesh);

    Vector2 position = {0};
    assert(mesh_setAttribute(&mesh, "position", &position));

    while (!window_shouldClose(window)) {
        window_pollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        position = (Vector2) {
            sin(glfwGetTime()) * 0.5,
            cos(glfwGetTime()) * 0.5,
        };

        mesh_setAttribute(&mesh, "position", &position);
        mesh_render(&mesh);

        window_swapBuffers(&window);
    }

    // Cleanup resources
    free(vertexPositions);
    free(indices);
    free(vertexColors);
    freeImage(&image);
    window_close(&window);
    mesh_free(&mesh);

    return 0;
}

void initOpenGLSettings(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    setDrawLineWidth(2);
    glClearColor(0.5, 0.7, 0.7, 1);
}

Window createWindow(void) {
    WindowSettings ws = WINDOW_SETTINGS_DEFAULT;
    return init_window(&ws);
}

