#pragma once

class VAO
{
    unsigned int id;

public:
    void bind();
    void unbind();
    VAO();
    ~VAO();
};
