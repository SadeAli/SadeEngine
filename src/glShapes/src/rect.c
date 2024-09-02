#include <glad/glad.h>
#include "vec.h"

void drawRect(Vec2 pos, Vec2 size) {
    const float vertices[] = {
        pos.x - size.x*0.5f, pos.y + size.y*0.5f,  // top left 
        pos.x + size.x*0.5f, pos.y + size.y*0.5f,  // top right
        pos.x - size.x*0.5f, pos.y - size.y*0.5f,  // bottom left
        pos.x + size.x*0.5f, pos.y - size.y*0.5f,  // bottom right
    };
    const u32 vertexCount = 4;

    const u32 indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        1, 3, 2    // second triangle
    };  
    const u32 indexCount = 6;

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2)*vertexCount, vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32)*indexCount , indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);  

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
