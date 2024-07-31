#pragma once

#include <string>

class Texture
{
    unsigned int id;

public:
    Texture(std::string const& filename);
    void bind();
};
