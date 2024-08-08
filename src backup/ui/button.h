#pragma once

#include <raylib.h>
#include <stdio.h>

typedef struct Button_t
{
    Rectangle rect;
    Color color;

    bool isDown : 1;

    void (*onPress)(struct Button_t *b);
    void (*onRelease)(struct Button_t *b);
} Button;

void buttonTestPressAction(Button *b)
{
    b->color = GREEN;
    b->isDown = true;
}

void buttonTestReleaseAction(Button *b)
{
    b->color = RED;
    b->isDown = false;
}

void buttonDraw(const Button *b)
{
    DrawRectangleRec(b->rect, b->color);
}

void updateButton(Button *b)
{
    const Vector2 mousePosition = GetMousePosition();

    //
    const bool isHover = CheckCollisionPointRec(mousePosition, b->rect);
    const bool isMouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool isMouseDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    const bool clicked = !b->isDown && isHover && isMouseClicked;
    const bool justReleased = b->isDown && !(isMouseDown && isHover);

    if (clicked && b->onPress != NULL) b->onPress(b);
    if (justReleased && b->onRelease != NULL) b->onRelease(b);
}