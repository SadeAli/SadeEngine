
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel = mat4(1);
uniform mat4 uView = mat4(1);
uniform mat4 uProjection = mat4(1);

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    // gl_Position = mat4(1) * vec4(aPos, 1.0);
}
