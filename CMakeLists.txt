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
    main.cc
    callbacks.cc
)
target_include_directories(
    main
    PUBLIC
    glad/include
)
target_link_libraries(
    main
    glfw
    glad
)
