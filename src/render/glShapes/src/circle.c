#include "glad/glad.h"

#include <math.h>
#include <stdlib.h>

// Circle generation function
void generateCircleVertices(int segments, float radius, float *vertices) {
    int vertexCount = segments + 2;  // 1 center point + 'segments' perimeter points
    int index = 0;

    // Center point (always at (0, 0))
    vertices[index++] = 0.0f;
    vertices[index++] = 0.0f;

    // Perimeter points
    for (int i = 0; i <= segments; ++i) {
        float angle = (2.0f * M_PI * i) / segments;
        vertices[index++] = radius * cos(angle);
        vertices[index++] = radius * sin(angle);
    }
}

// Function to create a VAO and VBO for circle vertices
void createCircleVBO(int segments, GLuint *vao, GLuint *vbo) {
    float *vertices = (float *)malloc((segments + 2) * 2 * sizeof(float));  // 2 coords per vertex (x, y)
    generateCircleVertices(segments, 1.0f, vertices);  // Generate circle with radius 1.0

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, (segments + 2) * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(vertices);
}

void renderCircles(GLuint vao, GLuint vbo, int segments) {
    // Use instance rendering to draw multiple circles in a single call
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);  // Render the circle
    glBindVertexArray(0);
}
