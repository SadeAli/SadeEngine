#include "glad/glad.h"
#include "window.h"
#include "GLFW/glfw3.h"
#include <assert.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>

static bool glfwStarted = 0;

Window init_window(const WindowSettings ws[static 1])
{
    // only 1 window is permitted
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

    // TODO: change glfw swap interval (1) with an optional version
    //
    // enable frame cap
    glfwSwapInterval(1);

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

bool window_shouldClose(Window *window) {
    if (window->glfwWindow == nullptr) {
        return true;
    }

    return glfwWindowShouldClose(window->glfwWindow);
}

void window_pollEvents() {
    glfwPollEvents();
}

void window_swapBuffers(Window *window) {
    glfwSwapBuffers(window->glfwWindow);

    const double currentTime = glfwGetTime();

    window->frameTime = currentTime - window->lastFrameUpdate;
    window->lastFrameUpdate = currentTime;

    double timePerFrame = 1000.0f / window->settings.fps;
    if (window->frameTime < timePerFrame) {
        double timeUntilNextFrame = timePerFrame - window->frameTime;

        struct timespec sleepTime;
        sleepTime.tv_sec = (time_t)(timeUntilNextFrame / 1000); // Full seconds
        sleepTime.tv_nsec = (long)((timeUntilNextFrame - (sleepTime.tv_sec * 1000)) * 1000000); // Remainder in nanoseconds

        thrd_sleep(&sleepTime, nullptr);
    }
}

double window_getFrameTime(Window *window) {
    return window->frameTime;
}
