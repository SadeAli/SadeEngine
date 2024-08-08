#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <assert.h>

typedef struct RelativePosition2D
{
    Vector2 position;
    int child_capacity;
    int child_count;
    struct RelativePosition2D *children;
} RelativePosition2D;

typedef RelativePosition2D rp2d;

bool rp2d_init(Vector2 position, int child_capacity, int child_count, RelativePosition2D children)
{
    return false;
}