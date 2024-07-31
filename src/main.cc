#include "texture.h"
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../arrayalgebra/arrayalgebra.h"
#include "callbacks.h"
#include "ebo.h"
#include "shader.h"
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
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "openGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    int constexpr default_window_width = 800;
    int constexpr default_window_height = 600;
    glViewport(0, 0, default_window_width, default_window_height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // This scope is to prevent glfwTerminate from being called before
    // destructors which causes segfaults (i think)
    {
        VAO vao{};
        vao.bind();

        Texture texture(
            "../res/mctexture/assets/minecraft/textures/block/dirt.png"
        );

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
        Shader vertex_shader1("../res/basic_vertex.glsl", GL_VERTEX_SHADER);
        Shader fragment_shader1(
            "../res/basic_fragment.glsl",
            GL_FRAGMENT_SHADER
        );
        ShaderProgram shader1(
            vertex_shader1.get_id(),
            fragment_shader1.get_id()
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_NEAREST
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        while (!glfwWindowShouldClose(window)) {
            process_input(window);

            // Go Forth and RENDER
            glClear(GL_COLOR_BUFFER_BIT);
            shader1.use();
            texture.bind();
            vao.bind();
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
