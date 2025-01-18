#ifndef MESH_TEMPLATE_H
#define MESH_TEMPLATE_H

#include <glad/glad.h>
#include <stdlib.h>

typedef struct {
    GLuint      location;       // Attribute location in the shader
    GLuint      numComponents;  // Number of components (e.g., 2 for vec2, 3 for vec3, etc.)
    GLenum      type;           // Type of the attribute (GL_FLOAT, GL_INT, etc.)
    GLboolean   normalized;     // Whether the attribute should be normalized
} GLVertexAttribute;

typedef struct Mesh_t {
    // TODO: add public private indicator (_)
    
    // gpu storage
    GLuint              vao;                // Vertex Array Object
    GLuint              vbo;                // Vertex Buffer Object
    GLuint              ebo;                // Element Buffer Object (for indices)
    GLuint              textureID;          // Texture ID, if using textures

    // data
    void                *vertexData;        // Pointer to vertex attribute data
    size_t              vertexCount;        // Number of vertices
    unsigned int        *indices;           // Indices to draw
    size_t              indexCount;         // Number of indices

    // layout
    GLVertexAttribute   *attributes;        // List of attributes
    size_t              attributeCount;     // Number of attributes

    // cache
    size_t              vertexSize;         // Size of a single vertex in bytes (for stride calculation)
} Mesh;

void mesh_addVertexAttribute(Mesh *mesh, GLuint location, GLuint size, GLenum type, GLboolean normalized);
void mesh_assignInterleavedData(Mesh *mesh, void **attribute_data);
void mesh_uploadToGPU(Mesh *mesh);
void mesh_render(Mesh *mesh);
void mesh_free(Mesh *mesh);

#endif
