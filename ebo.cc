#include <glad/glad.h>

#include "ebo.h"

EBO::EBO(unsigned int* data, std::size_t size, unsigned int draw_type)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        size * sizeof(unsigned int),
        data,
        draw_type
    );
}
EBO::~EBO() { glDeleteBuffers(1, &id); }

void EBO::bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
