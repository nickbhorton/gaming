cmake_minimum_required(VERSION 3.29)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(
    dumbgameengine 
    LANGUAGES CXX C
)
# builds the glfw binaries
add_subdirectory(
    glfw
)

add_library(
    glad
    glad/src/glad.c
)
target_include_directories(
    glad 
    PUBLIC
    glad/include
)

add_executable(
    main
    src/main.cc
    src/callbacks.cc
    src/shader.cc
    src/vbo.cc
    src/vao.cc
    src/ebo.cc
    src/texture.cc
    src/glfw_wrapper.cc
)
target_include_directories(
    main
    PUBLIC
    glad/include
    stb
)
target_link_libraries(
    main
    glfw
    glad
)
