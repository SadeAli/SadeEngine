#include <sadeEngine.h>

typedef struct Scene2D_t {
    Color color;
} Scene2D;

void drawScene2D(Scene2D *s, Window *window) {
    clearBG(s->color);
}

int main() {
    WindowSettings ws = WINDOW_SETTINGS_DEFAULT;
    Window window = init_window(&ws);

    Scene2D scene;
    scene.color = {50, 90, 110, 255};

    while (!window_shouldClose(window)) {
        window_pollEvents();

        drawScene2D(&scene, &window);
        window_swapBuffers(&window);
    }

    window_close(&window);
}
