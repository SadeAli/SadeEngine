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
    Window window = init_windowDefault();

    ShaderFile sList[] = {
        {"resources/shaders/hello.frag", SHADER_TYPE_FRAGMENT},
        {"resources/shaders/hello.vert", SHADER_TYPE_VERTEX},
    };

    OpenglShaderProgram shader = init_openglShaderProgram(sList, 2);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glBindVertexArray(0);

    glClearColor(55 / 255.0, 18 / 255.0, 18 / 255.0, 1);
    while(!window_shouldClose(&window)) {
        // NOTE: handle logic here

        // render start
        glClear(GL_COLOR_BUFFER_BIT);

        // NOTE: render here
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window_swapBuffers(&window);
        // render end
        window_pollEvents();
    }

    return 0;
}

