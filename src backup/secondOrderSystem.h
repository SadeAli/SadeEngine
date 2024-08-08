#pragma once

#include <raylib.h>

typedef struct SecondOrderSystem_t
{
    float initialResponse; // user friendly alternative to set internal coefficients
    float frequency;       // user friendly alternative to set internal coefficients
    float damping;         // user friendly alternative to set internal coefficients

    float kxd; // input velocity internal coefficient

    float x;  // input position
    float xd; // input velocity

    float kyd;  // output velocity internal coefficient
    float kydd; // output acceleration internal coefficient

    float y;   // output position
    float yd;  // output velocity
    float ydd; // output acceleration
} SecondOrderSystem;

void secondOrderSystemInput(SecondOrderSystem *s, float x, float xd);
void secondOrderSystem_getCoefficients(SecondOrderSystem s[static 1]);
void solveSecondOrderSystem(SecondOrderSystem s[static 1], float x, float xd, float T);

#define INIT_SECOND_ORDER_SYSTEM(ir, f, damp, ...)                  \
    (SecondOrderSystem)                                             \
    {                                                               \
        .initialResponse = ir, .frequency = f, .damping = damp,     \
        .kyd = damp / (PI * f),                                     \
        .kydd = 1 / ((2 * PI * f) * (2 * PI * f)),                  \
        .kxd = ir * damp / (2 * PI * f), __VA_ARGS__                \
    }