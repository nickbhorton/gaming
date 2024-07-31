#include <fstream>
#include <iostream>

#include <glad/glad.h>

#include "shader.h"

Shader::Shader(std::string const& filename, unsigned int shader_type)
{
    std::ifstream file{};

    file.open(filename);
    if (!file.good()) {
        std::cerr << "failed to find file: " << filename << "\n";
        return;
    }
    std::string line{};
    while (std::getline(file, line)) {
        text.append(line);
        text.push_back('\n');
    }
    file.close();
    id = glCreateShader(shader_type);
    const char* text_data = text.data();
    const char* const* text_data_address = &text_data;
    glShaderSource(id, 1, text_data_address, NULL);
    glCompileShader(id);
    int success;
    char infolog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infolog);
        std::cerr << "error in shader compilation: " << infolog;
        std::exit(1);
    }
}

auto Shader::get_id() const -> unsigned int { return id; }

void Shader::set_float(std::string const& name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

// This deletes the vertex and fragment shaders because deconstructors are
// funky. Be careful
ShaderProgram::ShaderProgram(
    unsigned int vertex_shader_id,
    unsigned int fragment_shader_id
)
{
    id = glCreateProgram();
    glAttachShader(id, vertex_shader_id);
    glAttachShader(id, fragment_shader_id);
    glLinkProgram(id);
    int success;
    char infolog[512];
    glGetProgramiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infolog);
        std::cerr << "error in shader program linking: " << infolog;
        std::exit(1);
    }
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}
ShaderProgram::~ShaderProgram() { glDeleteProgram(id); }

void ShaderProgram::use() { glUseProgram(id); }

auto ShaderProgram::get_id() const -> unsigned int { return id; }
