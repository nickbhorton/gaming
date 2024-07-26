#pragma once

#include <string>

class Shader
{
    std::string text;
    unsigned int id;

public:
    auto get_id() const -> unsigned int;
    Shader(std::string const& filename, unsigned int shader_type);
};

class ShaderProgram
{
    unsigned int id;

public:
    void use();
    ShaderProgram(
        unsigned int vertex_shader_id,
        unsigned int fragment_shader_id
    );
    ~ShaderProgram();
};
