#include <glad/glad.h>

#include "vao.h"
VAO::VAO() { glGenVertexArrays(1, &id); }
VAO::~VAO() {}
void VAO::bind() { glBindVertexArray(id); }
void VAO::unbind() { glBindVertexArray(0); }
