#version 330 core
out vec4 FragColor;

uniform vec4 vertex_color;

void main() {
    FragColor = vertex_color;
}
