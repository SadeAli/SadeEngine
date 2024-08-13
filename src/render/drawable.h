#pragma once

typedef struct Drawable_t {
    unsigned int vao;
    unsigned int elementCount;
    unsigned int shader;
} Drawable;

void drawable_draw(Drawable d);
unsigned int init_rect_vao();
unsigned int init_cube_vao_textured();
