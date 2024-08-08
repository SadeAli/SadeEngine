#include "glad/glad.h"
#include "window.h"
#include "GLFW/glfw3.h"
#include <assert.h>

static bool glfwStarted = 0;

Window init_window(const WindowSettings ws[static 1])
{
    assert(!glfwStarted);
    glfwInit();
    glfwStarted = true;

    GLFWwindow *glfwWindow = nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(ws->width , ws->height, ws->title, nullptr, nullptr);
    assert(glfwWindow != nullptr);

    // glfwSetWindowAttrib(glfwWindow, GLFW_RESIZABLE, 1);
    glfwMakeContextCurrent(glfwWindow);
    
    // load opengl functions
    int version = gladLoadGL();
    assert (version != 0);
    glViewport(0, 0, ws->width, ws->height);

    // glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return (Window) {
        .glfwWindow = glfwWindow,
        .settings = *ws,
    };
}

Window init_windowDefault() {
    WindowSettings ws = {
        .title = "myLeaf",
        .width = 1920,
        .height = 1080,
        .fps = 60
    };

    return init_window(&ws);
}

bool window_shouldClose(Window window) {
    if (window.glfwWindow == nullptr) {
        return true;
    }

    return glfwWindowShouldClose(window.glfwWindow);
}

void window_pollEvents() {
    glfwPollEvents();
}
