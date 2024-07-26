#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "arrayalgebra/arrayalgebra.h"
#include "callbacks.h"
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

        std::vector<aa::vec3> vertexes =
            {{-0.5, -0.5, 0.0}, {0.5, -0.5, 0.0}, {0.0, 0.5, 0.0}};
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

        Shader vertex_shader("../basic_vertex.glsl", GL_VERTEX_SHADER);
        Shader fragment_shader("../basic_fragment.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shader(vertex_shader.get_id(), fragment_shader.get_id());
        shader.use();
        vao.bind();

        while (!glfwWindowShouldClose(window)) {
            process_input(window);

            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
