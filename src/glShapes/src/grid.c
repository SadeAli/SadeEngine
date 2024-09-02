#include "grid.h"
#include "render/shader.h"
#include "defines.h"
#include "stdio.h"
#include "stdlib.h"
#include "vec.h"

void drawGrid(OpenglShader s) {
    int radius = 5;
    float cellSize = 0.5;

    const u32 linesCount = 2 * (2 * radius + 1);
    const u32 verticesPerLine = 2;
    const u32 bufferSize = sizeof(Vec3) * verticesPerLine * linesCount;

    Vec3 *vertices = malloc(bufferSize);
    if (vertices == NULL) {
        perror("malloc");
        return;
    }

    u32 placed = 0;

    // Horizontal lines
    for (int z = -radius; z <= radius; z++) {
        vertices[placed++] = (Vec3){-radius * cellSize, 0, z * cellSize};
        vertices[placed++] = (Vec3){radius * cellSize, 0, z * cellSize};
    }

    // Vertical lines
    for (int x = -radius; x <= radius; x++) {
        vertices[placed++] = (Vec3){x * cellSize, 0, -radius * cellSize};
        vertices[placed++] = (Vec3){x * cellSize, 0, radius * cellSize};
    }

    u32 vao, vbo;

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Copy vertex data to buffer
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STREAM_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Use shader program
    glUseProgram(s);

    // Draw the grid
    glDrawArrays(GL_LINES, 0, placed);

    // Cleanup
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    free(vertices);
}
