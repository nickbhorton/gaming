#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "arrayalgebra/arrayalgebra.h"
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
        VAO vao1{};
        vao1.bind();

        std::vector<aa::vec3> vertexes1 =
            {{-1, -1, 0.0}, {0, -1, 0.0}, {0, 0, 0.0}};
        VBO vbo1(
            (char*)vertexes1.data(),
            vertexes1.size() * sizeof(vertexes1[0]),
            GL_STATIC_DRAW
        );
        vbo1.bind();

        std::vector<unsigned int> indicies = {0, 1, 2};
        EBO ebo(indicies.data(), indicies.size(), GL_STATIC_DRAW);
        ebo.bind();

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(vertexes1[0]),
            (void*)0
        );
        glEnableVertexAttribArray(0);

        VAO vao2{};
        vao2.bind();

        std::vector<aa::vec3> vertexes2 = {{0, 0, 0.0}, {1, 1, 0}, {1, 0, 0.0}};
        VBO vbo2(
            (char*)vertexes2.data(),
            vertexes2.size() * sizeof(vertexes2[0]),
            GL_STATIC_DRAW
        );
        vbo2.bind();
        ebo.bind();

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(vertexes2[0]),
            (void*)0
        );
        glEnableVertexAttribArray(0);

        vao2.unbind();

        // Shader stuff
        Shader vertex_shader1("../basic_vertex.glsl", GL_VERTEX_SHADER);
        Shader fragment_shader1("../basic_fragment.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shader1(
            vertex_shader1.get_id(),
            fragment_shader1.get_id()
        );

        Shader vertex_shader2("../basic_vertex.glsl", GL_VERTEX_SHADER);
        Shader fragment_shader2("../basic_fragment2.glsl", GL_FRAGMENT_SHADER);
        ShaderProgram shader2(
            vertex_shader2.get_id(),
            fragment_shader2.get_id()
        );

        while (!glfwWindowShouldClose(window)) {
            process_input(window);

            // Uniform stuff
            float time = glfwGetTime();
            float green_val = (std::sin(time) / 2.0) + 0.5;
            int vertex_color_location =
                glGetUniformLocation(shader1.get_id(), "vertex_color");
            shader1.use();
            glUniform4f(vertex_color_location, 0, green_val, 0, 1);

            // Go Forth and RENDER
            glClear(GL_COLOR_BUFFER_BIT);
            vao1.bind();
            shader1.use();
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
            vao2.bind();
            shader2.use();
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
