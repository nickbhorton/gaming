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

aa::vec3 camera_pos{0, 0, 10};
aa::vec3 const camera_dir{0, 0, -1};
aa::vec3 const camera_up{0, 1, 0};
aa::vec3 const camera_right{aa::cross(camera_dir, camera_up)};

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    float const cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos = camera_pos + cameraSpeed * camera_dir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos = camera_pos - cameraSpeed * camera_dir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos = camera_pos - aa::normalize(aa::cross(camera_dir, camera_up)
                                  ) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos = camera_pos + aa::normalize(aa::cross(camera_dir, camera_up)
                                  ) * cameraSpeed;
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
    glEnable(GL_DEPTH_TEST);

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

    /*
    vertex_push(vertexes, {-1, 1, -1}, uv_bl, color2);
    vertex_push(vertexes, {1, 1, 1}, uv_tr, color2);
    vertex_push(vertexes, {1, 1, -1}, uv_br, color2);
    vertex_push(vertexes, {-1, 1, -1}, uv_bl, color2);
    vertex_push(vertexes, {-1, 1, 1}, uv_tl, color2);
    vertex_push(vertexes, {1, 1, 1}, uv_tr, color2);
    */

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

    std::cout << camera_right << "\n";

    aa::vec3 const cube_pos{0, 0, 0};

    aa::mat4 const model = {
        {{1, 0, 0, cube_pos[0]},
         {0, 1, 0, cube_pos[1]},
         {0, 0, 1, cube_pos[2]},
         {0, 0, 0, 1}}
    };
    aa::mat4 const view = {
        {{camera_right[0], camera_right[1], camera_right[2], 0},
         {camera_up[0], camera_up[1], camera_up[2], 0},
         {camera_dir[0], camera_dir[1], camera_dir[2], 0},
         {0, 0, 0, 1}}
    };
    float const n = 0.1;
    float const f = 100;
    float const r = 1;
    float const l = -1;
    float const t = 1;
    float const b = -1;
    aa::mat4 const projection = {
        {{2.0f * n / (r - l), 0, (r + l) / (r - l), 0},
         {0, 2.0f * n / (t - b), (t + b) / (t - b), 0},
         {0, 0, -(f + n) / (f - n), -2.0f * f * n / (f - n)},
         {0, 0, -1, 0}}
    };

    shader.use();
    unsigned int transform_location =
        glGetUniformLocation(shader.get_id(), "transform");

    while (!glfwWindowShouldClose(glfw.get_window())) {
        process_input(glfw.get_window());

        // Go Forth and RENDER
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        aa::mat4 const cpos = {
            {{1, 0, 0, -camera_pos[0]},
             {0, 1, 0, -camera_pos[1]},
             {0, 0, 1, -camera_pos[2]},
             {0, 0, 0, 1}}
        };

        aa::mat4 transform = screen * projection * view * cpos * model;

        // this is sketchy and depends on imlementation of aa::mat4 and std
        // array but for now works
        glUniformMatrix4fv(
            transform_location,
            1,
            GL_FALSE,
            (const float*)&transform
        );

        texture.bind();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

        glfwSwapBuffers(glfw.get_window());
        glfwPollEvents();
    }
}
