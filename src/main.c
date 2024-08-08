#define GLFW_INCLUDE_NONE

#include <assert.h>
#include <raymath.h>

#include "window.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

struct Engine
{
    WindowSettings windowSettings;
} engine = {
    .windowSettings =
        {
            .fps = 60,
            .height = 1080,
            .width = 1920,
            .title = "myLeaf",
        },
};

int main(void)
{
    // TODO: implement a good window manager
    Window _window = init_windowDefault();
    GLFWwindow *window = _window.glfwWindow;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(18 / 255.0, 18 / 255.0, 18 / 255.0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    return 0;
}
