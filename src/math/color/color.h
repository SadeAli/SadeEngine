#pragma once

#define RED (Color){255, 0, 0, 0}

typedef struct Colorf_t {
    float r, g, b, a;
} Colorf;

typedef struct Color_t {
    int r, g, b, a;
} Color;

Colorf toColorf(Color c);
