#pragma once

#include "glad/glad.h"

typedef unsigned int OpenglShaderProgram;
typedef unsigned int OpenglShader;

enum ShaderType {
    SHADER_TYPE_FRAGMENT = GL_FRAGMENT_SHADER,
    SHADER_TYPE_VERTEX = GL_VERTEX_SHADER,
};

typedef struct ShaderFile_t {
    char *path;
    enum ShaderType type;
} ShaderFile;

OpenglShader init_openglShader(const char *shaderPath, enum ShaderType shaderType);
OpenglShaderProgram init_openglShaderProgram(ShaderFile sList[static 1], int shaderCount);
