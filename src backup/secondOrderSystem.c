#pragma once

#include "secondOrderSystem.h"
#include <raylib.h>

void secondOrderSystem_getCoefficients(SecondOrderSystem s[static 1])
{
    s->kyd = s->damping / (PI * s->frequency);
    s->kydd = 1 / ((2 * PI * s->frequency) * (2 * PI * s->frequency));
    s->kxd = s->initialResponse * s->damping / (2 * PI * s->frequency);
}

void solveSecondOrderSystem(SecondOrderSystem s[static 1], float x, float xd, float T)
{
    if (!T) T = 0.00000001; 
    
    s->x = x;
    s->xd = xd / T;

    s->y += s->yd * T;
    s->ydd = (s->x + (s->kxd * s->xd) - s->y - (s->kyd * s->yd)) / s->kydd;
    s->yd += s->ydd * T;
}