#pragma once

#include <raylib.h>
#include "relativePosition.h"
#include "vec.h"

typedef struct Dragable2D_t
{
    RelativePosition2D *position;
    Rectangle collisionRect;

    vec2 dragOffset;
    bool beingDragged : 1;
} Dragable2D;

Dragable2D init_dragable2d(rp2d *pos, vec2 size);
void dragable2d_update(Dragable2D *m);
void dragable2d_draw(Dragable2D *m);
bool dragable2d_addChild(Dragable2D *parent, Dragable2D child);
