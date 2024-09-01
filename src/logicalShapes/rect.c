#include "rect.h"
#include "vec.h"

Rect init_rect(Vec2 position, Vec2 size)
{
    return (Rect) {
        .position = position,
        .size = size,
    };
}
