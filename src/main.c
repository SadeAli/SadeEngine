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
    // TODO: implement a decent window manager
    Window window = init_windowDefault();

    // TODO: initializers

    glClearColor(18 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    while(!window_shouldClose(window)) {
        // TODO: handle logic here

        // render start
        glClear(GL_COLOR_BUFFER_BIT);

        window_swapBuffers(window);
        // render end
        window_pollEvents();
    }

    return 0;
}
