#pragma once

#include "vec.h"
#include <math.h>

typedef struct Mat4_t {
    vec4
        m1,
        m2,
        m3,
        m4;
} Mat4;


Mat4 init_projection_matrix() {
    const float fov = 60;
    float s = 1 / tan(fov / 2 * M_PI / 180);

    const float far = 200;
    const float near = 0.01;

    const float a = -(far / (far - near));

    return (Mat4){
        .m1.x = s,
        .m2.y = s,
        .m3.z = a,
        .m3.w = -1,
        .m4.z = a * near,
    };
}

S_INLINE Mat4 mat4_transpose(Mat4 mat) {
    return (Mat4){
       .m1.data = {mat.m1.x, mat.m2.x, mat.m3.x, mat.m4.x},
       .m2.data = {mat.m1.y, mat.m2.y, mat.m3.y, mat.m4.y},
       .m3.data = {mat.m1.z, mat.m2.z, mat.m3.z, mat.m4.z},
       .m4.data = {mat.m1.w, mat.m2.w, mat.m3.w, mat.m4.w}
    };
}

S_INLINE Mat4 mat4_unit() {
    return (Mat4){
        .m1.x = 1,
        .m2.y = 1,
        .m3.z = 1,
        .m4.w = 1,
    };
}

S_INLINE Mat4 rotate(Vec3 rot) {

}
