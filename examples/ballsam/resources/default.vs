#version 330 core

layout(location = 0) in vec2 aPos; // The position of the vertex
layout(location = 1) in vec3 aColor; // The color of the vertex

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    ourColor = (aColor == vec3(0.0)) ? vec3(0.85,0.7,0.77) : aColor;
}
