#define GLFW_INCLUDE_NONE

#include <assert.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "glad/glad.h"

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

#include "render/shader.h"

int main(void)
{
    // TODO: implement a decent window manager
    Window window = init_windowDefault();

    // TODO: move this to parameters
    ShaderFile sList[] = {
        {"resources/shaders/test.frag", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/test.vert", SHADER_TYPE_VERTEX},
    };

    // TODO: initializers
    OpenglShaderProgram shader = init_openglShaderProgram(sList, 2);

    glClearColor(55 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    while(!window_shouldClose(&window)) {
        // TODO: handle logic here

        // render start
        glClear(GL_COLOR_BUFFER_BIT);

        window_swapBuffers(&window);
        // render end
        window_pollEvents();
    }

    return 0;
}

