#include "mesh2d.h"

void initGlMesh2D(GlMesh2D *mesh) {
    // Generate VAO
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    // Generate VBO
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * sizeof(*mesh->vertices), mesh->vertices, GL_STATIC_DRAW);

    // Generate EBO (Element Buffer Object)
    glGenBuffers(1, &mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(*mesh->vertices), (void*)0); // Position attribute
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*mesh->vertices), (void*)(sizeof(GLfloat) * 2)); // Texture coordinates
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);
}

void renderGlMesh2D(GlMesh2D *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void freeGlMesh2D(GlMesh2D *mesh) {
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteVertexArrays(1, &mesh->vao);
}
