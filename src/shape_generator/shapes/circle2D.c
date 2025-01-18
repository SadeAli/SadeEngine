#include <vector2.h>
#include <math.h>

#include "circle2D.h"

void generateCircleVertices2D(Vector2 *vertices, int segmentCount, float radius) {
    vertices[0] = (Vector2){0};
    float sliceAngle = 2 * 3.1415 / segmentCount;
    int offset = 1;
    for (int i = 0; i < segmentCount; i++) {
        vertices[offset] = (Vector2){
            radius * cos(i * sliceAngle),
            radius * sin(i * sliceAngle)
        };
        offset++;
    }
}

void generateCircleIndices(unsigned int *indices, int segmentCount) {
    int offset = 0;
    for (int i = 0; i < segmentCount; i++) {
        indices[offset++] = 0;
        indices[offset++] = i + 1;
        indices[offset++] = i + 2;
    }
    // wrap around
    indices[offset - 1] = 1;
}
