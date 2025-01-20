#version 330 core

layout(location = 0) in vec2 aPos;      // The position of the vertex
layout(location = 1) in vec3 aColor;    // The color of the vertex

out vec3 ourColor;              // output a color to the fragment shader

uniform float aspect_ratio = 16.0 / 10.0;     // Aspect ratio (width / height)

uniform vec2 uPosition = vec2(0.5, 0.5);

void main()
{
    vec2 position = aPos + uPosition;
    gl_Position = vec4(position.x, position.y * aspect_ratio, 0.0, 1.0);
    ourColor = (aColor == vec3(0.0)) ? vec3(0.85,0.7,0.77) : aColor;
}
