#pragma once

#include <string>

class Shader
{
    std::string text;
    unsigned int id;

public:
    auto get_id() const -> unsigned int;
    void set_float(std::string const& name, float value);
    Shader(std::string const& filename, unsigned int shader_type);
};

class ShaderProgram
{
    unsigned int id;

public:
    void use();
    auto get_id() const -> unsigned int;
    ShaderProgram(
        unsigned int vertex_shader_id,
        unsigned int fragment_shader_id
    );
    ~ShaderProgram();
};
