#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <GLFW/glfw3.h>

Texture Texture_Create(std::string path, int& width, int& height, int& numChannels)
{
    Texture ret;
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return ret;
    }
    else
    {
        std::cout << "Failed to load texture " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }
}

void Texture_Delete(Texture texture)
{
    glDeleteTextures(1, &texture);
}

JTexture::JTexture(std::string path)
{
    texture = Texture_Create(path, width, height, numChannels);
}

void JTexture::Use()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

JTexture::~JTexture()
{
    Texture_Delete(texture);
}