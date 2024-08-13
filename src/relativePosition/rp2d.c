#include "arrayList.h"
#include "defines.h"
#include "relativePosition.h"
#include "vec.h"

RelativePosition2D init_rp2d(vec2 relativePos)
{
    return (rp2d){
        .relativePosition = relativePos,
    };
}

bool rp2d_addChild(rp2d *parent, rp2d child)
{
    // TODO: replace this macro with proper function
    if (APPEND_ARRAY_LIST(parent->children, child) == false)
    {
        return false;
    }

    rp2d_recalculate(parent);

    return true;
}

void rp2d_recalculate(rp2d *rp)
{
    if (!rp->children.data)
        return;

    rp2d *const children = rp->children.data;
    for (u32 i = 0; i < rp->children.count; i++)
    {
        rp2d_setAnchorPosition(&children[i], rp2d_globalPosition(rp));
    }
}

void rp2d_setRelativePosition(RelativePosition2D *rp, const vec2 pos)
{
    rp->relativePosition = pos;
    rp2d_recalculate(rp);
}

void rp2d_setAnchorPosition(RelativePosition2D *rp, const vec2 pos)
{
    rp->anchor = pos;
    rp2d_recalculate(rp);
}

vec2 rp2d_globalPosition(const rp2d *rp)
{
    return vec2Add(rp->anchor, rp->relativePosition);
}
