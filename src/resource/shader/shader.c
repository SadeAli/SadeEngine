#include "shader.h"
#include "fileBuffer.h"
#include <glad/glad.h>
#include <stdio.h>

ShaderProgram construct_shaderProgram(Shader *shaders, int shaderCount) {
    // create shader
    OpenglShaderProgram shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        return 0;
    }

    for (int i = 0; i < shaderCount; i++) {
        glAttachShader(shaderProgram, shaders[i]);
    }

    glLinkProgram(shaderProgram);

    int success = 0;
    static char infoLog[GL_INFO_LOG_LENGTH];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("shaderProgram LINK_FAILED:\n");
        printf("\t%s\n", infoLog);

        glDeleteProgram(shaderProgram);
        return 0;
    }

    return shaderProgram;
}

Shader construct_shader(const char *shaderPath, ShaderType shaderType) {
    // compile individual shaders
    OpenglShader shader = glCreateShader(shaderType);
    if (shader == 0) {
        return 0;
    }

    // open shader file
    FILE *shaderFile = fopen(shaderPath, "r");
    if (!shaderFile) {
        perror("open file");
        glDeleteShader(shader);
        return 0;
    }

    // read shader
    FileBuffer sourceFB = init_FileBuffer(shaderFile);
    fclose(shaderFile);
    if (!sourceFB.data) {
        glDeleteShader(0);
        return 0;
    }

    const int lenght = sourceFB.size;
    glShaderSource(shader, 1, (const char **)&sourceFB.data, &lenght);
    glCompileShader(shader);

    free_FileBuffer(sourceFB);

    // compile shader
    // WARN: static variable is bad here
    // TODO: make static infolog dynamic or parametric
    static char infoLog[GL_INFO_LOG_LENGTH];
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("shader compilation failed\n"
           "%s\n"
           "for: %s\n",
           infoLog, shaderPath);
    return 0;
    }

    return shader;
}

void shaderProgram_use(ShaderProgram s) {
    glUseProgram(s);
}
