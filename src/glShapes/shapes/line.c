#include <glad/glad.h>

#include "defines.h"
#include "math/vec.h"

void drawLine3D(Vec3 a, Vec3 b) {
    u32 vao;
    u32 vbo;

    const Vec3 line[2] = {a, b};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(Vec3), line, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return;
}
