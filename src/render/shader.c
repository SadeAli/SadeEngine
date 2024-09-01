#include "shader.h"
#include "glad/glad.h"
#include <stdio.h>
#include "io/file.h"

OpenglShaderProgram init_openglShaderProgram(ShaderFile sList[static 1], int shaderCount) {
    // create shader
    OpenglShaderProgram shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        return 0;
    }

    for (int i = 0; i < shaderCount; i++) {
        OpenglShader shader = init_openglShader(sList[i].path, sList[i].type);

        if (!shader) {
            glDeleteProgram(shaderProgram);
        }

        glAttachShader(shaderProgram, shader);
        glDeleteShader(shader);
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

OpenglShader init_openglShader(const char *shaderPath, enum ShaderType shaderType) {
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

