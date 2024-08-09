#include <array>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../arrayalgebra/arrayalgebra.h"
#include "callbacks.h"
#include "glfw_wrapper.h"
#include "shader.h"
#include "texture.h"
#include "vao.h"
#include "vbo.h"

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void vertex_push(
    std::vector<std::array<float, 8>>& vertexes,
    aa::vec3 pos,
    aa::vec2 uv,
    aa::vec3 color
)
{
    vertexes.push_back(
        {pos[0], pos[1], pos[2], color[0], color[1], color[2], uv[0], uv[1]}
    );
}

int main()
{
    int constexpr default_window_width = 800;
    int constexpr default_window_height = 600;
    GlfwWrapper glfw{default_window_width, default_window_height};

    glfwMakeContextCurrent(glfw.get_window());
    glfwSetFramebufferSizeCallback(
        glfw.get_window(),
        framebuffer_size_callback
    );

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    glViewport(0, 0, default_window_width, default_window_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    VAO vao{};
    vao.bind();

    std::vector<aa::vec3> vertex_positions = {
        {1, 1, 1},
        {1, 1, -1},
        {1, -1, 1},
        {1, -1, -1},
        {-1, 1, 1},
        {-1, 1, -1},
        {-1, -1, 1},
        {-1, -1, -1},
    };

    aa::vec3 color1 = {1, 1, 1};
    aa::vec3 color2 = {1, 0, 0};

    aa::vec2 uv_bl = {0, 0};
    aa::vec2 uv_br = {1, 0};
    aa::vec2 uv_tl = {0, 1};
    aa::vec2 uv_tr = {1, 1};

    std::vector<std::array<float, 8>> vertexes{};
    vertex_push(vertexes, {-1, -1, 1}, uv_bl, color1);
    vertex_push(vertexes, {1, 1, 1}, uv_tr, color1);
    vertex_push(vertexes, {1, -1, 1}, uv_br, color1);
    vertex_push(vertexes, {-1, -1, 1}, uv_bl, color1);
    vertex_push(vertexes, {-1, 1, 1}, uv_tl, color1);
    vertex_push(vertexes, {1, 1, 1}, uv_tr, color1);

    vertex_push(vertexes, {-1, 1, -1}, uv_bl, color2);
    vertex_push(vertexes, {1, 1, 1}, uv_tr, color2);
    vertex_push(vertexes, {1, 1, -1}, uv_br, color2);
    vertex_push(vertexes, {-1, 1, -1}, uv_bl, color2);
    vertex_push(vertexes, {-1, 1, 1}, uv_tl, color2);
    vertex_push(vertexes, {1, 1, 1}, uv_tr, color2);

    VBO vbo(
        (char*)vertexes.data(),
        vertexes.size() * sizeof(vertexes[0]),
        GL_STATIC_DRAW
    );
    vbo.bind();

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertexes[0]),
        (void*)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertexes[0]),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertexes[0]),
        (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // Shader stuff
    Shader vertex_shader("../res/basic_vertex.glsl", GL_VERTEX_SHADER);
    Shader fragment_shader("../res/basic_fragment.glsl", GL_FRAGMENT_SHADER);
    ShaderProgram shader(vertex_shader.get_id(), fragment_shader.get_id());

    Texture texture("../res/dirt.png");
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_NEAREST
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    aa::mat4 const screen = {
        {{(float)1078 / (float)1918, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 1}}
    };

    aa::vec3 const cube_pos{0, 0, -5};
    aa::mat4 const model = {
        {{1, 0, 0, -cube_pos[0]},
         {0, 1, 0, -cube_pos[1]},
         {0, 0, 1, -cube_pos[2]},
         {0, 0, 0, 1}}
    };
    aa::mat4 const view = {
        {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
    };

    aa::mat4 transform = screen * view * model;

    shader.use();
    unsigned int transform_location =
        glGetUniformLocation(shader.get_id(), "transform");

    // this is sketchy and depends on imlementation of aa::mat4 and std array
    // but for now works
    glUniformMatrix4fv(
        transform_location,
        1,
        GL_FALSE,
        (const float*)&transform
    );

    while (!glfwWindowShouldClose(glfw.get_window())) {
        process_input(glfw.get_window());

        // Go Forth and RENDER
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        texture.bind();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

        glfwSwapBuffers(glfw.get_window());
        glfwPollEvents();
    }
}
