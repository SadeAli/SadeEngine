#pragma once

#include "defines.h"
#include "vec.h"

typedef struct TrailLine2D_t
{
    // line storage
    struct
    {
        u32 max;
        u32 index;
        vec2 *data;
    } points;

} TrailLine2D;

void trailLine2d_draw(const TrailLine2D *t);
void trailLine2d_nextPoint(TrailLine2D *t, vec2 point);
TrailLine2D init_trailLine2d(u32 maxSize);
