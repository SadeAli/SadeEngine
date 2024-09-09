#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>

#include "shader.h"
#include "fileBuffer.h"

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
        printf("shaderProgram LINK_FAILED:\n"
               "\t%s\n", infoLog);

        glDeleteProgram(shaderProgram);
        return 0;
    }

    return shaderProgram;
}

Shader construct_shaderFromFile(const char *path, ShaderType type) {
    // open shader file
    FILE *shaderFile = fopen(path, "r");
    if (!shaderFile) {
        perror("open file");
        return 0;
    }

    // read shader
    FileBuffer sourceFB = construct_fileBuffer(shaderFile);
    fclose(shaderFile);
    if (!sourceFB.data) {
        return 0;
    }

    Shader shader = construct_shaderFromSource(sourceFB.data, sourceFB.size, type);
    destruct_fileBuffer(sourceFB);

    return shader;
}

Shader construct_shaderFromSource(const char *source, int sourceLenght, ShaderType type) {
    // compile individual shaders
    Shader shader = glCreateShader(type);
    if (shader == 0) {
        return 0;
    }

    // compile
    glShaderSource(shader, 1, (const char **)&source, &sourceLenght);
    glCompileShader(shader);

    // check for errors
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        // print error log
        char *infoLog = malloc(GL_INFO_LOG_LENGTH * sizeof(char));
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("shader(%d) compilation failed:\n"
               "    %s\n", shader, infoLog);
        free(infoLog);

        // clear shader
        glDeleteShader(shader);
    }

    return shader;
}

void shaderProgram_use(ShaderProgram s) {
    glUseProgram(s);
}
