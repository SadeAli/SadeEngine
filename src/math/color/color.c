#include "color.h"

Colorf toColorf(Color c) {
    return (Colorf){
        .r = c.r / 255.0,
        .g = c.g / 255.0,
        .b = c.b / 255.0,
        .a = c.a / 255.0,
    };
}
