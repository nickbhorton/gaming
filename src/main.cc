#include <array>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// #include "../arrayalgebra/arrayalgebra.h"
#include "callbacks.h"
#include "ebo.h"
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

    std::vector<std::array<float, 8>> vertexes = {
        {0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},   // top right
        {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f},  // bottom right
        {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, // bottom left
        {-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}   // top left
    };

    VBO vbo(
        (char*)vertexes.data(),
        vertexes.size() * sizeof(vertexes[0]),
        GL_STATIC_DRAW
    );
    vbo.bind();

    std::vector<unsigned int> indicies = {0, 1, 2, 0, 3, 2};
    EBO ebo(indicies.data(), indicies.size(), GL_STATIC_DRAW);
    ebo.bind();

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

    while (!glfwWindowShouldClose(glfw.get_window())) {
        process_input(glfw.get_window());

        // Go Forth and RENDER
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        texture.bind();
        vao.bind();
        glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(glfw.get_window());
        glfwPollEvents();
    }
}
