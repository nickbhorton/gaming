#include "glfw_wrapper.h"
#include <iostream>

GlfwWrapper::GlfwWrapper(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "openGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }
}

GlfwWrapper::~GlfwWrapper()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

auto GlfwWrapper::get_window() -> GLFWwindow* { return window; }
