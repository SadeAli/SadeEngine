
#include <raylib.h>
#include "vec.h"
#include "trailLine2d.h"
#include <stdlib.h>
#include <string.h>

void trailLine2d_draw(const TrailLine2D *t) {
        DrawLineStrip((Vector2 *)t->points.data, t->points.index, GREEN);
        if (t->points.index != 0)
        {
            DrawLineV(t->points.data[0].raylibV, t->points.data[t->points.max - 1].raylibV, GREEN);
        }
        DrawLineStrip((Vector2 *)t->points.data + t->points.index, t->points.max - t->points.index, GREEN);
}

void trailLine2d_nextPoint(TrailLine2D *t, vec2 point) {
    t->points.data[t->points.index] = point;
    t->points.index = (t->points.index + 1) % t->points.max;
}

TrailLine2D init_trailLine2d(u32 maxSize) {
    TrailLine2D t = {0};

    t.points.data = calloc(maxSize, sizeof(vec2));
    t.points.max = (t.points.data == nullptr) ? 0 : maxSize;

    return t;
}
