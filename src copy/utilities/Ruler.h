#pragma once

#include <raylib.h>
#include <raymath.h>

typedef struct Ruler
{
    Vector2 p1;
    Vector2 p2;
    bool isMeasuring : 1;
} Ruler;

static inline void updateRuler(Ruler *ruler)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        ruler->isMeasuring = true;
        ruler->p1 = GetMousePosition();
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        ruler->isMeasuring = false;
    }
}

static inline void drawRuler(Ruler *r)
{
    Ruler ruler = *r;

    if (ruler.isMeasuring)
    {
        ruler.p2 = GetMousePosition();

        const Vector2 p1 = ruler.p1;
        const Vector2 p2 = ruler.p2;

        DrawText(TextFormat("%20.5f", Vector2Distance(ruler.p1, ruler.p2)),
                 (p1.x + p2.x) / 2, (p1.y + p2.y) / 2, 10, GREEN);
        DrawLine(p1.x, p1.y, p2.x, p2.y, GREEN);

        DrawCircle(p1.x, p1.y, 5, GREEN);
        DrawCircle(p2.x, p2.y, 5, GREEN);
    }
}