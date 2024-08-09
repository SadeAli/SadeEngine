#pragma once

#include <GLFW/glfw3.h>

typedef struct WindowSettings
{
    char *title;

    // size
    struct
    {
        int width;
        int height;
    };

    int fps;
} WindowSettings;

typedef struct Window_t {
    GLFWwindow *glfwWindow;

    struct {
        double lastFrameUpdate;
        double frameTime;
    };

    WindowSettings settings;

} Window;

/**
 * @brief an initializer for window calls init window with default window settings
 */
#define INIT_WINDOW(...)                                                                                               \
    initWindow(&(WindowSettings){.width = 1920, .height = 1080, .fps = 60, .title = "myLeaf", __VA_ARGS__})

// window settings related
WindowSettings init_windowSettingsDefault();

// window related
Window init_window(const WindowSettings ws[static 1]);
Window init_windowDefault();
bool window_shouldClose(Window *window);
void window_swapBuffers(Window *window);
double window_getFrameTime(Window *window);
void window_pollEvents();
