#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos.x, 2, aPos.y, 1.0f);
}
