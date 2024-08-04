#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv_coords;

uniform sampler2D the_texture;

void main() {
    FragColor = texture(the_texture, uv_coords) * vec4(color, 1.0);
}
