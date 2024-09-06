#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

typedef struct Vector2_t Vector2;

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

    ImGuiContext *igContext;

} Window;

/**
 * @brief an initializer for window calls init window with default window settings
 */
#define INIT_WINDOW(...)                                                                                               \
    init_window(&(WindowSettings){.width = 1920, .height = 1080, .fps = 60, .title = "myLeaf", __VA_ARGS__})

// window settings related
WindowSettings init_windowSettingsDefault();

// window related
Window init_window(const WindowSettings ws[static 1]);
Window init_windowDefault();
bool window_shouldClose(Window *window);
void window_swapBuffers(Window *window);
double window_getFrameTime(Window *window);
void window_pollEvents();
void window_close(Window *window);
Vector2 window_getMouseDelta(Window *window);
Vector2 window_getCursorPos(Window *window);
void window_hideCursor(Window *window);
void window_showCursor(Window *window);
void window_centerCursor(Window *window);
bool window_isKeyDown(Window *window, unsigned int key);
