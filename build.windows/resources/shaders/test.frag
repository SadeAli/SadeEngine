#version 450 core

out vec4 FragColor;
in vec3 fragPos;
in vec3 vertPos;

void main() {
  FragColor = vec4(vertPos / 50,1);
}