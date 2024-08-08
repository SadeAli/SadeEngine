#pragma once

#include <raylib.h>
#include <raymath.h>

typedef struct ivec2
{
    int x, y;
} ivec2;

static inline void Vector2Swap(Vector2 *v1, Vector2 *v2)
{
    const Vector2 tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}

static inline ivec2 Vector2IntegerDivision(Vector2 v, int i)
{
    return (ivec2){v.x / i, v.y / i};
}

static inline ivec2 ivec2DivByInt(ivec2 v, int i)
{
    return (ivec2){v.x / i, v.y / i};
}