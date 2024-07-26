#include "vbo.h"

#include <glad/glad.h>

VBO::VBO(char* data, std::size_t size, unsigned int draw_type)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, draw_type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
VBO::~VBO() { glDeleteBuffers(1, &id); }

void VBO::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }
