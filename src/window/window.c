#include <assert.h>
#include <stdio.h>
#include <threads.h>
#include <time.h>

#include <glad/glad.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include <cimgui_impl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "window.h"

#define KEY_PRESS GLFW_PRESS
#define KEY_RELEASE GLFW_RELEASE
#define KEY_REPEAT GLFW_REPEAT
#define MAX_KEYS 512

// Arrays to track key states
static char keyState[MAX_KEYS];

void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void window_resizeCallback(GLFWwindow* window, int width, int height);

void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key < 0 || key >= MAX_KEYS) {
        fprintf(stderr, "Invalid key: %d\n", key);
        return;  // Invalid key
    }

    keyState[key] = action;
}

void window_resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

struct Vector2_t {
    float x, y;
};

static inline Vector2 vec2Sub(Vector2 a, Vector2 b) {
    return (Vector2){
        .x = a.x - b.x,
        .y = a.y - b.y
    };
}

static bool glfwStarted = 0;

bool window_isKeyRepeat(Window *window, unsigned int key) {
    if (keyState[key] == KEY_REPEAT) {
        keyState[key] = KEY_RELEASE;
        return true;
    }

    return false;
}

bool window_isKeyPressed(Window *window, unsigned int key) {
    if (keyState[key] == KEY_PRESS) {
        keyState[key] = KEY_RELEASE;
        return true;
    }

    return false;
}

bool window_isKeyDown(Window *window, unsigned int key) {
    return glfwGetKey(window->glfwWindow, key);
}

void window_centerCursor(Window *window) {
    glfwSetCursorPos(window->glfwWindow, 0, 0);
}

void window_hideCursor(Window *window) {
    // glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void window_showCursor(Window *window) {
    glfwSetInputMode(window->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


Vector2 window_getCursorPos(Window *window) {
    double x, y;
    glfwGetCursorPos(window->glfwWindow, &x, &y);
    return (Vector2){(float)x, (float)y};
}

Vector2 window_getMouseDelta(Window *window) {
    static Vector2 oldPos = {0};
    const Vector2 newPos = window_getCursorPos(window);
    const Vector2 delta = vec2Sub(newPos, oldPos);
    oldPos = newPos;
    return delta;
}

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

    glfwSetWindowAttrib(glfwWindow, GLFW_RESIZABLE, 1);
    glfwMakeContextCurrent(glfwWindow);

    glfwSetWindowSizeLimits(glfwWindow, 16, 10, 2560, 1600);

    // TODO: make better pfs implementation
    // glfwSwapInterval(1);

    // load opengl functions
    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);;
    assert (version != 0);

    // NOTE: no need to use glViewport since ImGui_ImplGlfw_InitForOpenGL does that for us
    // glViewport(0, 0, 1920, 1080); 

    ImGuiContext *igContext = igCreateContext(nullptr);
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    igStyleColorsDark(NULL);

    glfwSetWindowSizeCallback(glfwWindow, window_resizeCallback);
    glfwSetKeyCallback(glfwWindow, window_key_callback);

    return (Window) {
        .glfwWindow = glfwWindow,
        .settings = *ws,
        .igContext = igContext,
    };
}

Window init_windowDefault() {
    WindowSettings ws = {
        .title = "sadeEngine",
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

void window_close(Window *window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(window->igContext);

    glfwDestroyWindow(window->glfwWindow);
    glfwTerminate();
}
