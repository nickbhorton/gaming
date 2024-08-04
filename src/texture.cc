#include <glad/glad.h>
#include <iostream>

#include "stb_image.h"
#include "texture.h"

Texture::Texture(std::string const& filename)
{
    glGenTextures(1, &id);

    int width, height, num_channels;
    unsigned char* img_data =
        stbi_load(filename.c_str(), &width, &height, &num_channels, 0);
    if (img_data) {
        // std::cout << "width: " << width << "\n";
        // std::cout << "height: " << height << "\n";
        // std::cout << "channels: " << num_channels << "\n";
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            img_data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "could not load texture: " << filename << "\n";
    }
    stbi_image_free(img_data);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, id); }
