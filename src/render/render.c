#include <glad/glad.h>
#include <color/color.h>
#include "render.h"

void clearBG(const Color c) {
    glClearColor(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
