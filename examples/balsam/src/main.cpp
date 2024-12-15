#include "sadeEngine.h"

typedef struct Color_t {
    int r, g, b, a;
} Color;

typedef struct Scene2D_t {
    Color color;
} Scene2D;

void drawScene2D(Scene2D *s, Window *window) {
    glClearColor(s->color.r / 255.0f, s->color.g / 255.0f, s->color.g / 255.0f,s->color.b / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window_swapBuffers(window);
}

int main() {
    WindowSettings ws = WINDOW_SETTINGS_DEFAULT;
    Window window = init_window(&ws);

    Scene2D scene;
    scene.color = {50, 90, 110, 255};

    while (!window_shouldClose(window)) {
        drawScene2D(&scene, &window);
        window_pollEvents();
    }

    window_close(&window);
}
