#include "vec.h"
#include "relativePosition.h"
#include "arrayDynamics.h"

RelativePosition3D init_rp3d(Vec3 relativePos)
{
    return (rp3d){
        .relativePosition = relativePos,
        .globalPosition = relativePos,
    };
}

bool rp3d_addChild(rp3d *parent, rp3d child)
{
    const bool appendSuccess = appendArrayDynamic(
        (void **)&parent->children.data, sizeof(rp3d), &parent->children.count, &parent->children.capacity, &child);

    if (appendSuccess == false)
    {
        return false;
    }

    rp3d_recalculate(parent);
    return true;
}

void rp3d_recalculate(rp3d *rp)
{
    if (!rp->children.data)
        return;

    rp3d *const children = rp->children.data;
    for (u32 i = 0; i < rp->children.count; i++)
    {
        rp3d_setGlobalPosition(&children[i], vec3Add(children[i].relativePosition, rp->globalPosition));
    }
}

void rp3d_setRelativePosition(RelativePosition3D *rp, const Vec3 pos) {
    const Vec3 delta = vec3Sub(pos, rp->relativePosition);
    rp->globalPosition = vec3Add(rp->globalPosition, delta);
    rp3d_recalculate(rp);
}

void rp3d_setGlobalPosition(RelativePosition3D *rp, const Vec3 pos)
{
    rp->globalPosition = pos;

    if (!rp->children.data)
        return;

    rp3d *const children = rp->children.data;
    for (u32 i = 0; i < rp->children.count; i++)
    {
        rp3d_setGlobalPosition(&children[i], vec3Add(children[i].relativePosition, rp->globalPosition));
    }
}

// not sure if complete
void rp3d_IncreasePosition(RelativePosition3D *rp, Vec3 delta)
{
    rp->relativePosition = vec3Add(rp->relativePosition, delta);

    if (!rp->children.data)
        return;

    RelativePosition3D *const children = rp->children.data;
    for (u32 i = 0; i < rp->children.count; i++)
    {
        rp3d_IncreasePosition(&children[i], delta);
    }
}
