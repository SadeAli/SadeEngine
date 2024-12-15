#pragma once
#include "shader.h"

typedef enum ShaderUniformType {
    UNDEFINED = 0,
    INTEGER,
    FLOAT,
    MATRIX4,
} ShaderUniformType;

typedef struct ShaderUniform {
    ShaderUniformType type;
    void *data;
    char *name;
} ShaderUniform;

void shaderUniform_apply(ShaderProgram shader, ShaderUniform uniform);
