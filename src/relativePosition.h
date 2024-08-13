#pragma once

#include "vec.h"
#include "arrayList.h"

// rp2d definition
typedef struct RelativePosition2D_t
{
    vec2 anchor;
    vec2 relativePosition;

    // struct
    // {
    //     struct RelativePosition2D_t *data;
    //     u32 capacity;
    //     u32 count;
    // } children;

    ARRAYLIST_COMPATIBLE_STRUCT(struct RelativePosition2D_t) children;

} RelativePosition2D;

// rp3d definition
typedef struct RelativePosition3D_t
{
    Vec3 globalPosition;
    Vec3 relativePosition;

    struct
    {
        struct RelativePosition3D_t *data;
        u32 capacity;
        u32 count;
    } children;

} RelativePosition3D;

// type alias
typedef RelativePosition2D rp2d;
typedef RelativePosition3D rp3d;

// rp2d functions
RelativePosition2D init_rp2d(vec2 relativePos);
bool rp2d_addChild(RelativePosition2D *parent, RelativePosition2D child);
void rp2d_recalculate(RelativePosition2D *rp);
void rp2d_setAnchorPosition(RelativePosition2D *rp, const vec2 pos);
void rp2d_IncreasePosition(RelativePosition2D *rp, vec2 delta);
void rp2d_setRelativePosition(RelativePosition2D *rp, const vec2 pos);
vec2 rp2d_globalPosition(const RelativePosition2D *rp);

// rp3d funcitons
RelativePosition3D init_rp3d(Vec3 relativePos);
bool rp3d_addChild(rp3d *parent, rp3d child);
void rp3d_recalculate(rp3d *rp);
void rp3d_setGlobalPosition(RelativePosition3D *rp, const Vec3 pos);
void rp3d_IncreasePosition(RelativePosition3D *rp, Vec3 delta);
void rp3d_setRelativePosition(RelativePosition3D *rp, const Vec3 pos);
