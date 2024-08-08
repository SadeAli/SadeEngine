#pragma once

#include <raylib.h>
#include <raymath.h>

typedef struct TopdownMover {
    Vector2 position;
    Rectangle collisionRect;
    float movementSpeed;
} TopdownMover;

void TopdownMoveUnstopable(TopdownMover *mover);
void TopdownMoverDraw(TopdownMover mover, Color c);