#include <iostream>

#include "callbacks.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "framebuffer_size_callback: " << width << " " << height
              << std::endl;
    glViewport(0, 0, width, height);
}
