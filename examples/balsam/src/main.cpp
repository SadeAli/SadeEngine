#include "sadeEngine.h"

int main() {
    WindowSettings ws = WINDOW_SETTINGS_DEFAULT;
    Window window = init_window(&ws);

    while (!window_shouldClose(window)) {
        window_pollEvents();
    }

    window_close(&window);
}
