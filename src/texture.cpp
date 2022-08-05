#include "texture.h"
#include "stb_image.h"
#include <iostream>

Texture Texture_Create(std::string path, int& width, int& height, int& numChannels)
{
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
    if (data)
    {
        Texture ret;
        glGenTextures(1, &ret);
        glBindTexture(GL_TEXTURE_2D, ret);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

JTexture::~JTexture()
{
    Texture_Delete(texture);
}