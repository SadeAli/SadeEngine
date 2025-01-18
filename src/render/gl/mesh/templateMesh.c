#include "templateMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t getTypeSize(GLenum type) {

    // TODO: add remaining types

    unsigned int typeSize = 0;
    switch (type) {
        case GL_UNSIGNED_INT:
            typeSize = sizeof(GLuint);
            break;
        case GL_INT:
            typeSize = sizeof(GLint);
            break;
        case GL_FLOAT:
            typeSize = sizeof(GLfloat);
            break;
        default:
            fprintf(stderr, "Unrecognized attribute type\n");
            typeSize = 0;
    };

    return typeSize;
}

void mesh_addVertexAttribute(Mesh *mesh, GLuint location, GLuint numComponents, GLenum type, GLboolean normalized) {
    // Reallocate memory for attributes
    typeof(mesh->attributes) tmpAttributes;
    tmpAttributes = realloc(mesh->attributes, sizeof(GLVertexAttribute) * (mesh->attributeCount + 1));

    if (tmpAttributes) {
        mesh->attributes = tmpAttributes;
    }
    else {
        perror("realloc");
        return;
    }

    GLVertexAttribute *new_attr = &mesh->attributes[mesh->attributeCount];

    // Set the attribute details
    new_attr->location = location;
    new_attr->numComponents = numComponents;
    new_attr->type = type;
    new_attr->normalized = normalized;

    // Update vertex_size to include this new attribute
    mesh->vertexSize += numComponents * getTypeSize(type);

    mesh->attributeCount++;
}

void mesh_assignInterleavedData(Mesh *mesh, void **attribute_data) {
    // Calculate total size of interleaved data
    size_t total_vertex_size = mesh->vertexCount * mesh->vertexSize;
    mesh->vertexData = malloc(total_vertex_size);

    // Interleave the data
    for (size_t i = 0; i < mesh->vertexCount; ++i) {
        size_t offset = 0;
        for (size_t j = 0; j < mesh->attributeCount; ++j) {
            // Get the size of the current attribute
            size_t attribute_size = mesh->attributes[j].numComponents * getTypeSize(mesh->attributes[j].type);

            // Copy the attribute data into the correct position in the interleaved buffer
            memcpy((char*)mesh->vertexData + i * mesh->vertexSize + offset,
                   (char*)attribute_data[j] + i * attribute_size,
                   attribute_size);

            // Move the offset forward by the size of the attribute
            offset += attribute_size;
        }
    }
}

void mesh_uploadToGPU(Mesh *mesh) {
    // Generate and bind a Vertex Array Object (VAO)
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    // Generate and bind a Vertex Buffer Object (VBO)
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

    // Upload the interleaved vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * mesh->vertexSize, mesh->vertexData, GL_STATIC_DRAW);

    // Generate and bind an Element Buffer Object (EBO) for indices
    glGenBuffers(1, &mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

    // Set up the vertex attribute pointers based on the attributes in the mesh
    size_t offset = 0;
    for (size_t i = 0; i < mesh->attributeCount; ++i) {
        GLVertexAttribute *attr = &mesh->attributes[i];

        // Specify the layout of the vertex data
        glVertexAttribPointer(attr->location, attr->numComponents, attr->type, attr->normalized,
                              mesh->vertexSize, (void*)(offset));

        // Enable the vertex attribute
        glEnableVertexAttribArray(attr->location);

        // Update the offset for the next attribute
        offset += attr->numComponents * getTypeSize(attr->type);
    }

    // Unbind the VAO to prevent accidental modification
    glBindVertexArray(0);
}

void mesh_free(Mesh *mesh) {
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteVertexArrays(1, &mesh->vao);
    if (mesh->textureID != 0) {
        glDeleteTextures(1, &mesh->textureID);
    }

    free(mesh->attributes);
    free(mesh->vertexData);
}

void mesh_render(Mesh *mesh) {
    glBindVertexArray(mesh->vao);

    // Bind texture if available
    if (mesh->textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    }

    // Draw the mesh using indexed rendering
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
