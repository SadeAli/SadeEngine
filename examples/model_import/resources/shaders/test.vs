#version 450 core

layout(location = 0) in vec3 aPos;

out vec3 fragPos;

uniform float time = 0.0001;
uniform mat4 projection = mat4(1);
uniform mat4 model = mat4(1);

void main() {
  gl_Position = projection * model * vec4(aPos, 1);
  fragPos = aPos + vec3(0.5);
}