#pragma once

#include "glad/glad.h"

typedef unsigned int OpenglShaderProgram;
typedef unsigned int OpenglShader;

enum OpenglShaderType {
    SHADER_TYPE_FRAGMENT = GL_FRAGMENT_SHADER,
    SHADER_TYPE_VERTEX = GL_VERTEX_SHADER,
};

typedef OpenglShaderProgram ShaderProgram;
typedef OpenglShader Shader;
typedef enum OpenglShaderType ShaderType;

typedef struct ShaderFile_t {
    char *path;
    ShaderType type;
} ShaderFile;

Shader construct_shaderFromFile(const char *path, ShaderType type);
Shader construct_shaderFromSource(const char *source, int sourceLenght, ShaderType type);
ShaderProgram construct_shaderProgram(Shader *shaders, int shaderCount);
void shaderProgram_use(ShaderProgram s);
