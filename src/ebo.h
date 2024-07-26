#pragma once

#include <cstddef>
class EBO
{
    unsigned int id;

public:
    void bind();
    EBO(unsigned int* data, std::size_t size, unsigned int draw_type);
    ~EBO();
};
