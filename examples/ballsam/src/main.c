
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
#include <templateMesh.h>

void initOpenGLSettings(void);
Window createWindow(void);
ShaderProgram loadShaders(void);

ShaderProgram loadShaders(void) {
    Shader fs = shader_loadFromFile("../resources/default.fs", SHADER_TYPE_FRAGMENT);
    Shader vs = shader_loadFromFile("../resources/default.vs", SHADER_TYPE_VERTEX);

    ShaderProgram sp = shaderProgram_fromShaders((Shader[]){fs, vs}, 2);

    shader_free(fs);
    shader_free(vs);
    return sp;
}

int main(void) {
    // Create Window
    Window window = createWindow();
    initOpenGLSettings();

    // Load Shaders
    ShaderProgram shaderProgram = loadShaders();

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
        vertexColors[i].x = (float)(rand() % 10000) / 10000;
        vertexColors[i].y = (float)(rand() % 10000) / 10000;
        vertexColors[i].z = (float)(rand() % 10000) / 10000;
    }

    Mesh mesh = {0};
    mesh_addVertexAttribute(&mesh, 0, 2, GL_FLOAT, GL_FALSE);
    mesh.vertexCount = vCount;
    mesh.indexCount = iCount;
    mesh.indices = indices;
    mesh_assignInterleavedData(&mesh, (void*[]){vertexPositions});

    mesh_uploadToGPU(&mesh);

    while (!window_shouldClose(window)) {
        window_pollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram_use(shaderProgram);
        mesh_render(&mesh);

        window_swapBuffers(&window);
    }

    // Cleanup resources
    free(vertexPositions);
    free(indices);
    window_close(&window);
    mesh_free(&mesh);

    return 0;
}

void initOpenGLSettings(void) {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2);
    glClearColor(0.5, 0.7, 0.7, 1);
}

Window createWindow(void) {
    WindowSettings ws = WINDOW_SETTINGS_DEFAULT;
    return init_window(&ws);
}

