#include "shader.h"
#include "glad/glad.h"
#include <stdio.h>

// TODO: seperate file io header
typedef struct FileBuffer_t {
    char *data;
    size_t size;
} FileBuffer;

FileBuffer init_FileBuffer(FILE *path);

// TODO: seperate temporary definitions (headers etc)
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
    static char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("shaderProgram LINK_FAILED:\n");
        printf("\t%s\n", infoLog);

        glDeleteProgram(shaderProgram);
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

    // compile shader
    // WARN: static variable is bad here
    // TODO: make static infolog dynamic or parametric
    static char infoLog[512];
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


// TODO: seperate file io source
#include <stdio.h>
#include <stdlib.h>

FileBuffer init_FileBuffer(FILE *file) {
    FileBuffer fb = {0};

    // get file size
    // WARN: I am not checking errors for these 2 lines
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);

    // allocate memory for buffer
    char *buffer = malloc(sizeof(char) * (filesize + 1));
    if (!buffer) {
        perror("malloc");
        return fb;
    }

    // read file from beginning to end
    rewind(file);
    fread(buffer, sizeof(char), filesize, file);
    if (ferror(file)) {
        perror("fread");
        free(buffer);
        return fb;
    }
    buffer[filesize] = '\0';

    // if everything successful set file buffer and return it
    // NOTE: we initialize fb to zero at the start in case something goes wrong
    fb.data = buffer;
    fb.size = filesize + 1;

    return fb;
}
