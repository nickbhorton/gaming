#pragma once

#include <cstddef>

class VBO
{
    unsigned int id;

public:
    void bind();
    VBO(char* data, std::size_t size, unsigned int draw_type);
    ~VBO();
};
