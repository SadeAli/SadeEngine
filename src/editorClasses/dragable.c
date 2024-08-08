#include "dragable.h"
#include "relativePosition.h"

Dragable2D init_dragable2d(rp2d *pos, vec2 size)
{
    const vec2 globalPos = rp2d_globalPosition(pos);
    
    return (Dragable2D){
        .collisionRect = {globalPos.x, globalPos.y, size.x, size.y},
        .position = pos,
    };
}

void dragable2d_update(Dragable2D *m)
{
    const vec2 mousePosition = {.raylibV = GetMousePosition()};

    const bool isHover = CheckCollisionPointRec(mousePosition.raylibV, m->collisionRect);
    const bool isMouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    if (!m->beingDragged && isHover && isMouseClicked)
    {
        // set state
        m->beingDragged = true;
        m->dragOffset = vec2Sub(m->position->relativePosition, mousePosition);
    }
    else if (m->beingDragged && isMouseDown)
    {
        // update position
        rp2d_setRelativePosition(m->position, vec2Add(mousePosition, m->dragOffset));

        const vec2 globalPos = rp2d_globalPosition(m->position);
        m->collisionRect.x = globalPos.x;
        m->collisionRect.y = globalPos.y;
    }
    else
    {
        // update state
        m->beingDragged = false;
    }
}

void dragable2d_draw(Dragable2D *m)
{
    const vec2 globalPos = rp2d_globalPosition(m->position);
    m->collisionRect.x = globalPos.x;
    m->collisionRect.y = globalPos.y;
    DrawRectangleLinesEx(m->collisionRect, 5, m->beingDragged ? GREEN : RED);
}

bool dragable2d_addChild(Dragable2D *parent, Dragable2D child)
{
    return rp2d_addChild(parent->position, *child.position);
}
