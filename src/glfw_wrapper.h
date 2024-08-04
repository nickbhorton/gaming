#pragma once

#include <GLFW/glfw3.h>

class GlfwWrapper
{
    GLFWwindow* window;
    int width;
    int height;

public:
    GlfwWrapper(int width, int height);
    ~GlfwWrapper();
    auto get_window() -> GLFWwindow*;
};
