#pragma once

#include "defines.h"

#ifdef RAYLIB_H
#include <raylib.h>
#endif

typedef union vec2_t
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

#ifdef RAYLIB_H
    Vector2 raylibV;
#endif
} vec2;

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

#ifdef RAYLIB_H
    Vector3 raylibV;
#endif
} Vec3;

typedef union vec4_t
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
} vec4;

// ===================================== vec2 ===============================================
S_INLINE vec2 init_vec2(f32 x, f32 y)
{
    return (vec2){
        .x = x,
        .y = y,
    };
}

S_INLINE vec2 vec2Sub(vec2 a, vec2 b)
{
    const vec2 result = {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };

    return result;
}

S_INLINE vec2 vec2Add(vec2 a, vec2 b)
{
    const vec2 result = {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };

    return result;
}

// ===================================== vec3 ===============================================
S_INLINE Vec3 init_vec3(f32 x, f32 y, f32 z)
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
