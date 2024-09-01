#pragma once

#include "vec.h"

typedef struct Rectangle_t
{
    Vec2 position;
    Vec2 size;
} Rectangle;

typedef Rectangle Rect;

Rect init_rect(Vec2 position, Vec2 size);
