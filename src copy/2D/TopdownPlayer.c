#include "TopdownPlayer.h"
#include <raylib.h>
#include <raymath.h>

void TopdownMoveUnstopable(TopdownMover *mover) {
    Rectangle rect = mover->collisionRect;
    Vector2 position = mover->position;
    const float movementSpeed = mover->movementSpeed;

    const float deltaTime = GetFrameTime();

    // get input direction
    const Vector2 inputDirection = (Vector2){
        .x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
        .y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W),
    };

    // scale input direction with movement speed to
    // get this frames movement
    const Vector2 motion =
        Vector2Scale(inputDirection, movementSpeed * deltaTime);

    // increase position by motion
    position = Vector2Add(position, motion);

    rect.x = position.x;
    rect.y = position.y;

    mover->collisionRect = rect;
    mover->position = position;
}

void TopdownMoverDraw(TopdownMover mover, Color c) {
    const Rectangle rect = mover.collisionRect;
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, c);
}