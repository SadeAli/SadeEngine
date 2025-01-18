#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <stdio.h>
#include <vector2.h>

typedef struct {
    GLuint      vao;         // Vertex Array Object
    GLuint      vbo;         // Vertex Buffer Object
    GLuint      ebo;         // Element Buffer Object (for indices)
    Vector2     *vertices;
    GLuint      *indices;
    size_t      vertexCount;
    size_t      indexCount;
} GlMesh2D;

void initGlMesh2D(GlMesh2D *mesh);
void freeGlMesh2D(GlMesh2D *mesh);
void renderGlMesh2D(GlMesh2D *mesh);

#endif
