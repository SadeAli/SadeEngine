#include "vec.h"
#include "rect.h"

Rect init_rect(vec2 position, vec2 size)
{
    return (Rect) {
        .position = position,
        .size = size,
    };
}
