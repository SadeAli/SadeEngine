#version 330 core
layout (location = 0) in vec2 aPos;

const vec2 screenSize = vec2(1920, 1080);

void main()
{
    gl_Position = vec4(aPos / screenSize, 0.0, 1.0);
}
