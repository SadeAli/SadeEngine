#pragma once

#include "vec.h"

typedef struct Rectangle_t
{
    vec2 position;
    vec2 size;
} Rectangle;

typedef Rectangle Rect;

Rect init_rect(vec2 position, vec2 size);
