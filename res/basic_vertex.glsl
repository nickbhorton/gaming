#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 uv;

out vec3 color;
out vec2 uv_coords;


void main() {
    gl_Position = vec4(pos, 1.0);
    color = col;
    uv_coords = uv;
}
