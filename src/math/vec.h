#pragma once

#include "defines.h"

typedef union Vec2_t
{
    struct
    {
        union
        {
            f32 x, u, s;
        };

        union
        {
            f32 y, v, t;
        };
    };

    f32 data[2];

} Vec2;

typedef union vec3_t
{
    struct
    {
        union
        {
            f32 x;
            f32 r;
        };

        union
        {
            f32 y;
            f32 g;
        };

        union
        {
            f32 z;
            f32 b;
        };
    };

    f32 data[3];

} Vec3;

typedef union Vec4_t
{
    struct
    {
        union
        {
            f32 x;
            f32 r;
        };

        union
        {
            f32 y;
            f32 g;
        };

        union
        {
            f32 z;
            f32 b;
        };

        union
        {
            f32 w;
            f32 a;
        };
    };

    f32 data[4];
} Vec4;

// ===================================== Vec2 ===============================================
S_INLINE Vec2 init_Vec2(f32 x, f32 y)
{
    return (Vec2){
        .x = x,
        .y = y,
    };
}

S_INLINE Vec2 vec2Sub(Vec2 a, Vec2 b)
{
    const Vec2 result = {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };

    return result;
}

S_INLINE Vec2 vec2Add(Vec2 a, Vec2 b)
{
    const Vec2 result = {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };

    return result;
}

// ===================================== vec3 ===============================================
S_INLINE Vec3 init_Vec3(f32 x, f32 y, f32 z)
{
    return (Vec3){
        .x = x,
        .y = y,
        .z = z,
    };
}

S_INLINE Vec3 vec3Sub(Vec3 a, Vec3 b)
{
    const Vec3 result = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };

    return result;
}

S_INLINE Vec3 vec3Add(Vec3 a, Vec3 b)
{
    const Vec3 result = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };

    return result;
}
