#ifndef SHAPE_GENERATOR_CIRCLE_2D
#define SHAPE_GENERATOR_CIRCLE_2D

#include <vector2.h>

void generateCircleVertices2D(Vector2 *vertices, int segmentCount, float radius);
void generateCircleIndices(unsigned int *indices, int segmentCount);

#endif
