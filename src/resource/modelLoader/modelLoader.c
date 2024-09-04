typedef unsigned int u32;

#include <assimp/cimport.h>
#include <assimp/include/assimp/postprocess.h>
#include <assimp/include/assimp/scene.h>

#include <glad/glad.h>
#include <stdio.h>

#include "modelLoader.h"
#include "../../render/drawable.h"

Drawable import_test() {
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
    glGenBuffers(2, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // Calculate and allocate memory for vertex buffer
    u32 totalVertexSize = 0;
    for (u32 i = 0; i < scene->mNumMeshes; i++) {
        totalVertexSize += scene->mMeshes[i]->mNumVertices * sizeof(*scene->mMeshes[i]->mVertices);
    }

    glBufferData(GL_ARRAY_BUFFER, totalVertexSize, NULL, GL_STATIC_DRAW);

    // Calculate and allocate memory for element buffer
    u32 totalElementSize = 0;
    for (u32 i = 0; i < scene->mNumMeshes; i++) {
        for (u32 j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
            totalElementSize += scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(*scene->mMeshes[i]->mFaces[j].mIndices);
        }
    }
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

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

    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // NOTE: after import
    aiReleaseImport(scene);

    GLenum error = 0;
    while ((error = glGetError())) {
        printf("error: %d\n", error);
    }

    return (Drawable){.vao = vao, .elementCount = eboOffset};
}
